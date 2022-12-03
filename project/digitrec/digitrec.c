// Include necessary libraries
#include <stdio.h>
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
// #include "libpng-1.6.37/png.h"
#include "hardware/uart.h"
#include "vga_graphics.h"

// order for greyscale:
// 0 1 2 4 3 5 6 7
// darkest - lightest

// Macros for fixed-point arithmetic (faster than floating point)
// #define multfix15(a,b) ((fix15)((((signed long long)(a))*((signed long long)(b)))>>15))
// #define float2fix15(a) ((fix15)((a)*32768.0)) 
// #define fix2float15(a) ((float)(a)/32768.0)
// #define absfix15(a) abs(a) 
// #define int2fix15(a) ((fix15)(a << 15))
// #define fix2int15(a) ((int)(a >> 15))
// #define char2fix15(a) (fix15)(((fix15)(a)) << 15)
// #define divfix(a,b) (fix15)( (((signed long long)(a)) << 15) / (b))


// Values output to DAC
int DAC_output_0 ;
int DAC_output_1 ;

// SPI data
uint16_t DAC_data_1 ; // output value
uint16_t DAC_data_0 ; // output value

// DAC parameters (see the DAC datasheet)
// A-channel, 1x, active
#define DAC_config_chan_A 0b0011000000000000
// B-channel, 1x, active
#define DAC_config_chan_B 0b1011000000000000

//SPI configurations
#define PIN_SCK 10
#define PIN_SDO 11
#define PIN_SDIN 12
#define PIN_CS 13
#define SPI_PORT spi1
// chip select macros
#define CS_on gpio_put(PIN_CS,0)
#define CS_off gpio_put(PIN_CS,1)

// FRAM commands
#define fram_write (2)
#define fram_read  (3)
#define fram_we    (6)
#define fram_wd    (4)

#define K_CONST 3

#define UART_ID1 uart1
#define BAUD_RATE 115200
#define UART_TX_PIN 4
#define UART_RX_PIN 5

int distance_euclidean( int a[784], int b[784] ) {
  int dist = 0;
  int interm = 0;

  // Iterate through array
  for ( int i = 0; i < 784; i++ ) {
    dist += abs( a[i] - b[i] );
    //dist += interm * interm;
  }

  // if ( input == test_num )
  //   printf("dist = %d\n", dist);
  
  return dist;
}

void update_knn( int test_inst[784], int train_inst[784], int min_distances[K_CONST]) {

  int dist = distance_euclidean( test_inst, train_inst );
  
  // replace minimum distance
  for ( int i = 0; i < K_CONST; i++ ) {
    if ( dist < min_distances[i] ) {
      int temp = min_distances[i];
      min_distances[i] = dist;
      dist = temp;
    }
  }
}

int knn_vote( int knn_set[10][K_CONST] ) {
  int k_dist[K_CONST];     // array of k smallest distances
  int digit_near[K_CONST]; // knn digits

  // printf("knn set:\n ");
  // for ( int i = 0; i < 10; i++ ) {
  //   printf("%d:", i);
  //   for ( int j = 0; j < K_CONST; j++ ) {
  //     printf("  %d", knn_set[i][j]);
  //   }
  //   printf("\n");
  // }

  // initialize
  for ( int i = 0; i < K_CONST; i++ ) {
    k_dist[i] = 785;
    digit_near[i] = 10;
  }

  // loop through each minimum distances
  for ( int i = 0; i < 10; i++ ) {
    for ( int j = 0; j < K_CONST; j++ ) {

      // assign distance and digit
      int dist  = knn_set[i][j];
      int digit = i;

      // loop through array of current k nearest neighbors
      for ( int k = 0; k < K_CONST; k++ ) {
        // replace if smaller
        if ( dist < k_dist[k] ) {
          // swap so we can check the other knn
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

  // printf("digit_near = [ ");

  // for ( int i = 0; i < K_CONST; i++ ) {
  //   printf("%d ", digit_near[i]);
  // }

  // printf(" ]\n");

  // count number of instances of digit
  int count[K_CONST];

  for ( int i = 0; i < K_CONST; i++ )
    count[i] = 0;

  for ( int i = 0; i < K_CONST; i++ )
    for ( int j = 0; j < K_CONST; j++ )
      if ( digit_near[i] == digit_near[j] )
        count[i] += 1; // if match, increment counter

  int largest_count = 0;
  int digit;

  // find the largest count (and if tie, take the first one)
  for ( int i = 0; i < K_CONST; i++ ) {
    if ( count[i] > largest_count ) {
      largest_count = count[i];
      digit = digit_near[i];
    }
  }

  return digit;
}

// void read_png( char *file_name, int img[784] ) {
//   FILE *fp = fopen(file_name, "rb");
//   png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
//   png_infop info_ptr = png_create_info_struct(png_ptr);  
//   png_init_io(png_ptr, fp);
//   png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
//   png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
//   int width = png_get_image_width(png_ptr,info_ptr);
//   int height = png_get_image_height(png_ptr,info_ptr);
//   //printf("width = %d, height = %d\n", width, height);
//   int count = 0;
//   for (int i = 0; i < height; i++) {
//     for (int j = 0; j < width * 4; j += 4) {
//       // if (row_pointers[i][j] == 0)
//       //   printf("%d,   ", row_pointers[i][j]);
//       // else
//       //   printf("%d, ", row_pointers[i][j]);
//       img[count] = row_pointers[i][j];
//       count++;
//     }   
//     // printf("\n");
//   }
//   png_destroy_read_struct(&png_ptr, NULL, NULL); 
//   fclose(fp);
// }

int val = 0;
char num[5];

// This thread runs on core 0
static PT_THREAD (protothread_uart0(struct pt *pt)) {
  // Indicate thread beginning
  PT_BEGIN(pt) ;

  // Input digit
  //static int num;
  int i = 0;

  while(1) {
    PT_YIELD_usec(100) ;
    // // user input
    // sprintf(pt_serial_out_buffer, "Input a file name: \n\r");
    // serial_write; // spawn a thread to do the non-blocking serial read
    // serial_read;
    
    // sscanf(pt_serial_in_buffer, "%d", &num);
    // // printf

    // sprintf(pt_serial_out_buffer, "num = %d\n\r", num);
    //uart_puts(UART_ID1, "uart1\n\r");

    //uart_puts(UART_ID1, "eh? ");

    if ( val ) {
      uart_puts(UART_ID1, "hello: ");
      for ( int i = 0; i < 5; i++ ) {
        uart_putc(UART_ID1, num[i]);
        uart_puts(UART_ID1, " ");
      }
      uart_puts(UART_ID1, "\n\r");
    }

    // sprintf(pt_serial_out_buffer, "Input a file name: \n\r");
    // serial_write; // spawn a thread to do the non-blocking serial read
    // serial_read;
    
    // sscanf(pt_serial_in_buffer, "%d", &num);
    
    // char i_str;
    // sprintf(i_str, "%d", i);
    //uart_puts(UART_ID1, "hello .....");
    // if ( uart_is_readable(UART_ID) ) 
    //   uart_puts(UART_ID1, "data available!\n\r");
    // else
    //   uart_puts(UART_ID1, "data not available\n\r");
    // uart_puts(UART_ID1, i_str);
    // uart_puts(UART_ID1, "\n\r");
    // i = i + 1;
    

    //uart_puts(UART_ID1, "num = %d\n", num);

    // FILE *f = fopen("file.txt", "w");
    // fprintf(f, "Some text: %d\n", num);
    // fprintf(f, "hello\n");

    // fclose(f);

    //int test_img[5];

    // for ( int i = 0; i < 5; i++ ) {
    //   //serial_read;  // convert input string to number
    //   // scanf blocks core
    //   // serial read does not block the other threads on the core
    //   // 38400 baud rate
    //   scanf(pt_serial_in_buffer,"%d", &num);
    //   test_img[i] = num;
    // }

    //int img[784];

    //read_png( file_name, img );

    // for ( int i = 0; i < 28; i++ ) {
    //   for ( int j = 0; j < 28; j++ ) {
    //     printf("%d ", img[i*j+j]);
    //   }
    //   printf("\n");
    // }

    // Choose a binary representation of the digit inputted
    //int input_digit[784] = testing_data[expected_digit];

    //-----------------------------

    // // This array stores K minimum distances per training set
    // int knn_set[10][K_CONST];

    // // Initialize the knn set
    // for ( int i = 0; i < 10; ++i )
    //   for ( int k = 0; k < K_CONST; ++k )
    //     knn_set[i][k] = 200000; // Max distance is 200000
    
    // // i is for data sets, and j is looping through each digit
    // for ( int i = 0; i < 10; ++i ) {
    //   for ( int j = 0; j < 10; j++ ) {
    //     //int training_instance[784] = training_data[j][i]; // Read a new instance from the training set
    //     update_knn( /*img*/, training_data[j][i], knn_set[j] ); // Update the KNN set
    //   }
    // }

    // int actual_digit = knn_vote( knn_set ); // Compute the final output

    // if ( actual_digit == 0 )
    //   printf("Success! %d == %d\n", actual_digit, 0);
    // else
    //   printf("Failure! %d != %d\n", actual_digit, 0);

    //---------------------------
  }

  // Indicate thread end
  PT_END(pt) ;
}

// This thread runs on core 1
static PT_THREAD (protothread_uart1(struct pt *pt)) {
  // Indicate thread beginning
  PT_BEGIN(pt) ;

  // Input digit
  //static char num[5];

  while(1) {
    //uart_puts(UART_ID, "uart0\n\r");
    PT_YIELD_usec(100) ;
    // user input
    // printf("Hello?? ----\n");
    // sprintf(pt_serial_out_buffer, "Input a file name: \n\r");
    // serial_write; // spawn a thread to do the non-blocking serial read
    // serial_read;
    
    //sscanf(pt_serial_in_buffer, "%c", &num);
    for ( int i = 0; i < 5; i++ ) {
      while(!uart_is_readable(UART_ID));
      //PT_YIELD_UNTIL(pt, (int)uart_is_readable(UART_ID)) ;
      uart_puts(UART_ID1, "reading");
      num[i] = uart_getc(UART_ID);
    }

    val = 1;
    uart_puts(UART_ID1, "changing val");
    if ( val )
      uart_puts(UART_ID1, "val changed");
    else
      uart_puts(UART_ID1, "val not changed");
    //uart_puts(UART_ID1, val);
    // fillRect(110, 110, 10, 10, 0);
    // fillRect(120, 110, 10, 10, 1);
    // fillRect(130, 110, 10, 10, 2);
    // fillRect(140, 110, 10, 10, 4);
    // fillRect(150, 110, 10, 10, 3);
    // fillRect(160, 110, 10, 10, 5);
    // fillRect(170, 110, 10, 10, 6);
    // fillRect(180, 110, 10, 10, 7);

    // for(int row = 0; row < 28; row++) {
    //   for(int col = 0; col < 28; col++) {
    //     if ( image[28*row + col] < 41 )
    //       fillRect(col*10 + 100, row*10 + 100, 10, 10, 0);
    //     else if ( image[28*row + col] < 83 )
    //       fillRect(col*10 + 100, row*10 + 100, 10, 10, 1);
    //     else if ( image[28*row + col] < 125 )
    //       fillRect(col*10 + 100, row*10 + 100, 10, 10, 2);
    //     else if ( image[28*row + col] < 167 )
    //       fillRect(col*10 + 100, row*10 + 100, 10, 10, 3);
    //     else if ( image[28*row + col] < 209 )
    //       fillRect(col*10 + 100, row*10 + 100, 10, 10, 6);
    //     else
    //       fillRect(col*10 + 100, row*10 + 100, 10, 10, 7);
    //   }
    // }
    
    // for ( int j = 0; j < 7; j++ )
    //   for ( int i = 0; i < 7; i++ ) 
    //     fillRect(110 + i*10, 40 + j*10, 10, 10, i);

    //uart_puts(UART_ID1, "hello!!! hello??? uart hello uart\n\r");

    //val = num;
  
  }

  // Indicate thread end
  PT_END(pt) ;
}

void core1_main(){
  // Add animation thread
  pt_add_thread(protothread_uart1) ;
  // Start the scheduler
  pt_schedule_start ;

}


// Core 0 entry point
int main() {

    // 1. Serial command 
    // // Initialize stdio/uart (printf won't work unless you do this!)
    stdio_init_all();

    // other UART
    //initVGA() ;

    uart_init(UART_ID1, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_fifo_enabled(UART_ID1, 1); // on by default actually...

    while(uart_is_readable(UART_ID)){uart_getc(UART_ID);}

    // //void draw() {
    // for(int row = 0; row < 28; row++) {
    //   for(int col = 0; col < 28; col++) {
    //     if ( image[28*row + col] > 100 )
    //       fillRect(row + 20, col + 20, 10, 10, WHITE);
    //     // fillRect(row + 20, col + 20, 10, 10, (image[28*row + col], image[28*row + col], image[28*row + col]));
    //   }
    // }
    //}

    // // Initialize SPI channel (channel, baud rate set to 20MHz)
    // spi_init(SPI_PORT, 20000000) ;
    // // Format (channel, data bits per transfer, polarity, phase, order)
    // spi_set_format(SPI_PORT, 16, 0, 0, 0);

    // // Map SPI signals to GPIO ports
    // gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    // gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    // gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    // gpio_set_function(PIN_CS, GPIO_FUNC_SPI) ;

    // // Map LDAC pin to GPIO port, hold it low (could alternatively tie to GND)
    // gpio_init(LDAC) ;
    // gpio_set_dir(LDAC, GPIO_OUT) ;
    // gpio_put(LDAC, 0) ;

    // // Map LED to GPIO port, make it low
    // gpio_init(LED) ;
    // gpio_set_dir(LED, GPIO_OUT) ;
    // gpio_put(LED, 0) ;

    // start core 1 
    multicore_reset_core1();
    multicore_launch_core1(&core1_main);

    // Add core 0 threads
    pt_add_thread(protothread_uart0) ;
    

    // Start scheduling core 0 threads
    pt_schedule_start ;

}