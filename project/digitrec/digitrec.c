// Include necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/sync.h"
#include "hardware/spi.h"
// Include protothreads
#include "pt_cornell_rp2040_v1.h"
#include "training_data.h"
#include "testing_data.h"
#include "hardware/uart.h"
#include "vga_graphics.h"

#define NUM_VAL 785

// order for greyscale:
// 0 1 2 4 3 5 6 7
// darkest - lightest

#define UART_ID1 uart1
#define BAUD_RATE 115200
#define UART_TX_PIN 4
#define UART_RX_PIN 5

#define K_CONST 3

// Initialize global variables
char num[NUM_VAL][3];      // Array of pixels in chars/strings
int  count[NUM_VAL];       // Number of chars per pixel e.g. 255 has 3 chars
int  actual_num[NUM_VAL];  // Pixel values in integers
char actual_char[NUM_VAL]; // Pixel values in char (8bits)
int  final_val = 0;        // All the pixels are valid

int abs( int val ) {
  if ( val < 0 )
    return -val;
  return val;
}

int distance_euclidean( char a[NUM_VAL], const char b[NUM_VAL] ) {
  int dist = 0;
  int interm = 0;

  // Iterate through array
  for ( int i = 0; i < NUM_VAL; i++ )
    dist += abs( (int)a[i] - (int)b[i] );
  
  return dist;
}

void update_knn( char test_inst[NUM_VAL], const char train_inst[NUM_VAL], int min_distances[K_CONST]) {

  int dist = distance_euclidean( test_inst, train_inst );
  
  // Replace minimum distance
  for ( int i = 0; i < K_CONST; i++ ) {
    if ( dist < min_distances[i] ) {
      int temp = min_distances[i];
      min_distances[i] = dist;
      dist = temp;
    }
  }
}

int knn_vote( int knn_set[10][K_CONST] ) {
  int k_dist[K_CONST];     // Array of k smallest distances
  int digit_near[K_CONST]; // knn digits

  for ( int i = 0; i < 10; i++ ) {
    printf("%d: ", i);
    for ( int j = 0; j < K_CONST; j++ ) {
      printf("%d ", knn_set[i][j]);
    }
    printf("\n");
  }

  // Initialize
  for ( int i = 0; i < K_CONST; i++ ) {
    k_dist[i] = 200000;
    digit_near[i] = 10;
  }

  // Loop through each minimum distances
  for ( int i = 0; i < 10; i++ ) {
    for ( int j = 0; j < K_CONST; j++ ) {

      // Assign distance and digit
      int dist  = knn_set[i][j];
      int digit = i;

      // Loop through array of current k nearest neighbors
      for ( int k = 0; k < K_CONST; k++ ) {
        // Replace if smaller
        if ( dist < k_dist[k] ) {
          // Swap so we can check the other knn
          int temp1 = k_dist[k];
          k_dist[k] = dist;
          dist = temp1;
          int temp2 = digit_near[k];
          digit_near[k] = digit;
          digit = temp2;
        }
      }
    }
  }

  // Count number of instances of digit
  int count[K_CONST];

  for ( int i = 0; i < K_CONST; i++ )
    count[i] = 0;

  for ( int i = 0; i < K_CONST; i++ )
    for ( int j = 0; j < K_CONST; j++ )
      if ( digit_near[i] == digit_near[j] )
        count[i] += 1; // If match, increment counter

  int largest_count = 0;
  int digit;

  // Find the largest count (and if tie, take the first one)
  for ( int i = 0; i < K_CONST; i++ ) {
    if ( count[i] > largest_count ) {
      largest_count = count[i];
      digit = digit_near[i];
    }
  }

  return digit;
}

// This thread runs on core 0
static PT_THREAD (protothread_uart0(struct pt *pt)) {
  // Indicate thread beginning
  PT_BEGIN(pt) ;

  while(1) {
    PT_YIELD_usec(100) ;

    // If entire image has been sent serially
    if ( final_val ) {
      fillRect(0, 0, 640, 480, 0); // Clear vga screen

      for ( int i = 0; i < NUM_VAL; i++) {
        // Convert to integer
        if ( count[i] < 2 ) {
          actual_num[i] = num[i][0] - '0';
          actual_char[i] = actual_num[i];
        } else {
          char str[3] = "";
          for ( int j = 0; j < count[i]; j++ )
            strncat(str, &num[i][j], 1);
          actual_num[i] = atoi(str);
          actual_char[i] = actual_num[i];
        }
      }

      // Print digit on screen
      for (int row = 0; row < 28; row++) {
        for(int col = 0; col < 28; col++) {
          if ( actual_num[28*row + col] < 41 )
            fillRect(col*10 + 180, row*10 + 50, 10, 10, 0);
          else if ( actual_num[28*row + col] < 83 )
            fillRect(col*10 + 180, row*10 + 50, 10, 10, 1);
          else if ( actual_num[28*row + col] < 125 )
            fillRect(col*10 + 180, row*10 + 50, 10, 10, 2);
          else if ( actual_num[28*row + col] < 167 )
            fillRect(col*10 + 180, row*10 + 50, 10, 10, 3);
          else if ( actual_num[28*row + col] < 209 )
            fillRect(col*10 + 180, row*10 + 100, 10, 10, 6);
          else
            fillRect(col*10 + 180, row*10 + 50, 10, 10, 7);
        }
      }

      // -----------------------------
      // knn algorithm
      // -----------------------------

      // This array stores K minimum distances per training set
      int knn_set[10][K_CONST];

      // Initialize the knn set
      for ( int i = 0; i < 10; ++i )
        for ( int k = 0; k < K_CONST; ++k )
          knn_set[i][k] = 200000; // Max distance is 200000
      
      // i is for training data sets, and j is looping through each digit
      for ( int i = 0; i < 10; ++i ) {
        for ( int j = 0; j < 10; j++ ) {
          //int training_instance[NUM_VAL] = training_data[j][i]; // Read a new instance from the training set
          update_knn( actual_char, training_data[j][i], knn_set[j] ); // Update the KNN set
        }
      }

      int actual_digit = knn_vote( knn_set ); // Compute the final output

      printf("Expected: %d\n", actual_digit);

      //-----------------------------

      static char str_num_boids_0[100];

      sprintf(str_num_boids_0, "Predicted Digit: %d", actual_digit);
      setCursor(215,400);
      setTextColor(111);
      setTextSize(2);
      writeString(str_num_boids_0);

      final_val = 0; // Do this once only
    }
  }

  // Indicate thread end
  PT_END(pt) ;
}

// This thread runs on core 1
static PT_THREAD (protothread_uart1(struct pt *pt)) {
  // Indicate thread beginning
  PT_BEGIN(pt) ;

  while(1) {
    // uart_puts(UART_ID, "uart0");
    // uart_puts(UART_ID1, "uart1");
    PT_YIELD_usec(100) ;

    // Iterate 
    for ( int i = 0; i < NUM_VAL; i++ ) {
      while(!uart_is_readable(UART_ID1));
      printf("reading %d\n", i);
      count[i] = uart_getc(UART_ID1) - '0';
      for ( int j = 0; j < count[i]; j++ )
        num[i][j] = uart_getc(UART_ID1);
    }
    final_val = 1;
  }

  // Indicate thread end
  PT_END(pt) ;
}

void core1_main(){
  pt_add_thread(protothread_uart1); // Add core 1 thread
  pt_schedule_start; // Start scheduling core 1 threads
}

// Core 0 entry point
int main() {
    stdio_init_all(); // Initialize stdio/uart
    initVGA() ;       // Initialize vga

    // Initialize other uart
    uart_init(UART_ID1, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_fifo_enabled(UART_ID1, 1);

    // Clear FIFO
    while(uart_is_readable(UART_ID1)){uart_getc(UART_ID1);}

    // Start core 1 
    multicore_reset_core1();
    multicore_launch_core1(&core1_main);

    pt_add_thread(protothread_uart0); // Add core 0 thread
    pt_schedule_start; // Start scheduling core 0 threads
}