#include "main.h"

/*This file will house all the logic on how to plot on the LCD screen, scale the graph, and
 * ultimately portay the data on the LCD.*/

/*This will convert the raw ADC data into real voltage values.*/
void Convert_Raw_Data(Coordinate *converted_data, uint16_t *raw_data){
	float ratio_to_convert = (3.3 / 65536);
	int i;

	for (i = 0; i < 320; i++) {
		converted_data->x = (float)i;
		converted_data->y = ((float)*raw_data * ratio_to_convert);

		raw_data++;
		converted_data++;
	}

	return;
}


/*This function will do the actual plotting of the data on the LCD screen. This is where you can control
 * the scaling of the graph as well. In the future I'll have more parameters or a function to change this.*/
void plot_pts(Coordinate pts[], float x_scale, float y_scale) {
	uint16_t x = 0, y = 0, prev_y = 0;			//Vars that will hold the actual pixel location on the lcd
	int i, k;
	float value, prev_value;					//Vars that will hold the actual x or y value of a pixel

	/*Calculating the smallest x and y units according to the given scales*/
	x_scale = 1.0/x_scale;
	y_scale = 1.0/y_scale;

	/*Logic to make the graph look smoother by choosing if a data pt should be plotted at the current x and y position*/
	for (i = 0; i < 320; i++) {
		value = 0;
		prev_value = 0;
		for (k = 0; k < 240; k++) {
			value = (float)k * y_scale;			//This should give us the value of the kth pixel

			if (pts[i].y == value){				//if you're equal then you're done
				y = k;
				break;
			}
			else if (pts[i].y < value){ 		//If you're less than, then you have to check if the previous value was closer to you
				if ((pts[i].y - prev_value) <= (value - pts[i].y)) {
					y = k - 1;
					break;
				}
				else {
					y = k;
					break;
				}
			}
			prev_value = value;
		}

		/*This is will actually plot the point on the LCD screen*/
		LCD_set_pixel(0xF000, x, prev_y);

		/*More logic to make the graph continuous*/
		while (y != prev_y) {
			if (y <= prev_y){
				prev_y--;
				LCD_set_pixel(0xF000, x, prev_y);
			}
			else {
				prev_y++;
				LCD_set_pixel(0xF000, x, prev_y);
			}
		}

		/*Incrementing to the next x and prev_y location the data will go to*/
		prev_y = y;
		x++;
	}

	return;
}
