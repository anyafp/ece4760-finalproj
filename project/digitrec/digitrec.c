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

void update_knn( digit_type test_inst, digit_type train_inst, int min_distances[K_CONST] ) {
  
  digit_type diff = test_inst ^ train_inst; // Compute the difference using XOR

  int dist = 0;
  // Count the number of set bits
  for ( int i = 0; i < 49; ++i )
    dist += diff[i];
  
  // replace minimum distance
  for ( int i = 0; i < K_CONST; i++ ) {
    if ( dist < min_distances[i] ) {
      int temp = min_distances[i];
      min_distances[i] = dist;
      dist = temp;
    }
  }
}

int knn_vote( bit6_t knn_set[10][K_CONST] ) {
  int k_dist[K_CONST];     // array of k smallest distances
  int digit_near[K_CONST]; // knn digits

  // initialize
  for ( int i = 0; i < K_CONST; i++ ) {
    k_dist[i] = 50;
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
          bit6_t temp1 = k_dist[k];
          k_dist[k] = dist;
          dist = temp1;
          int temp2 = digit_near[k];
          digit_near[k] = digit;
          digit = temp2;
        }
      }
    }
  }

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

// This thread runs on core 0
static PT_THREAD (protothread_core_0(struct pt *pt)) {
  // Indicate thread beginning
  PT_BEGIN(pt) ;

  // Input digit
  static int expected_digit;
  digit_type test[10] = { 0xe3664d8e00, 0x3041060800, 0xc041078000, 0x1c3c1858f00, 0x8363830600, 
                     0x6183030e00, 0x2081060400, 0x3c1830608,  0x6123850e00,  0xc103810204 }

  while(1) {
    // user input
    sprintf(pt_serial_out_buffer, "Input a digit: ");
    serial_write; // spawn a thread to do the non-blocking serial read
    serial_read;  // convert input string to number
    sscanf(pt_serial_in_buffer,"%d", &expected_digit);

    // Choose a binary representation of the digit inputted
    digit_type input_digit = test[expected_digit];

    // This array stores K minimum distances per training set
    int knn_set[10][K_CONST];

    // Initialize the knn set
    for ( int i = 0; i < 10; ++i )
      for ( int k = 0; k < K_CONST; ++k )
        knn_set[i][k] = 50; // Max distance is 49
    
    // i is for data sets, and j is looping through each digit
    for ( int i = 0; i < 100; ++i ) {
      for ( int j = 0; j < 10; j++ ) {
        digit_type training_instance = training_data[j][i]; // Read a new instance from the training set
        update_knn( input, training_instance, knn_set[j] ); // Update the KNN set
      }
    }

    int actual_digit = knn_vote( knn_set ); // Compute the final output

    if ( actual_digit == expected_digit )
      printf("Success! %d == %d\n", actual_digit, expected_digit);
    else
      printf("Failure! %d != %d\n", actual_digit, expected_digit);
  }

  // Indicate thread end
  PT_END(pt) ;
}




// Core 0 entry point
int main() {

    // 1. Serial command 
    // // Initialize stdio/uart (printf won't work unless you do this!)
    stdio_init_all();

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

    // Add core 0 threads
    pt_add_thread(protothread_core_0) ;

    // Start scheduling core 0 threads
    pt_schedule_start ;

}