/***********************************************************************
 * Project: Image Suite
 ***********************************************************************/ 

#include <stdio.h>
#include <MyroC.h>
#include <string.h>
#include <math.h>

//Sets one of the RGB components of "pic" to 0 depending on "color".
void
pixelStrip
(Picture * pic, char color)
{
  int end_row = (*pic).height;
  int end_col = (*pic).width;
  
  for(int row = 0; row < end_row; row++)
    {
      for(int col = 0; col < end_col; col++)
        {
          if (color == 'R')
            (*pic).pix_array[row][col].R = 0;
          else if (color == 'G')
            (*pic).pix_array[row][col].G = 0;
          else if (color == 'B')
            (*pic).pix_array[row][col].B = 0;
        }
    }
}


//Changes intensity of color channels, making the picture redder.
void
pictureRedder (Picture * pic)
{
  int end_row = (*pic).height;
  int end_col = (*pic).width;
  
  for(int row = 0; row < end_row; row++)
    {
      for(int col = 0; col < end_col; col++)
        {
          int red = (*pic).pix_array[row][col].R;
          int green = (*pic).pix_array[row][col].G;
          int blue = (*pic).pix_array[row][col].B;
          (*pic).pix_array[row][col].R = 255 - ((255 - red) * .40);
          (*pic).pix_array[row][col].G = blue * .40;
          (*pic).pix_array[row][col].B = green * .40;
        }
    }
}

//Changes intensity of color channels.making the picture greener.
void
pictureGreener (Picture * pic)
{
  int end_row = (*pic).height;
  int end_col = (*pic).width;
  
  for(int row = 0; row < end_row; row++)
    {
      for(int col = 0; col < end_col; col++)
        {
          int red = (*pic).pix_array[row][col].R;
          int green = (*pic).pix_array[row][col].G;
          int blue = (*pic).pix_array[row][col].B;
          (*pic).pix_array[row][col].R = red * .40;
          (*pic).pix_array[row][col].G = 255 - ((255 - green) * .40);
          (*pic).pix_array[row][col].B = blue * .40;
        }
    }
}

//Change intensity of color channels.making the picture bluer.
void
pictureBluer (Picture * pic)
{
  int end_row = (*pic).height;
  int end_col = (*pic).width;
  
  for(int row = 0; row < end_row; row++)
    {
      for(int col = 0; col < end_col; col++)
        {
          int red = (*pic).pix_array[row][col].R;
          int green = (*pic).pix_array[row][col].G;
          int blue = (*pic).pix_array[row][col].B;
          (*pic).pix_array[row][col].R = red * .40;
          (*pic).pix_array[row][col].G = green * .40;
          (*pic).pix_array[row][col].B = 255 - ((255 - blue) * .40);
        }
    }
}


// Select a circle of radius "radius" and center coordinates "xCenter" and yCenter" in "pic". Turn pixels in circle gray.
void
circleSelect (Picture * pic, int xCenter, int yCenter, int radius) 
{
  int end_row = (*pic).height;
  int end_col = (*pic).width;
  
  for(int row = 0; row < end_row; row++)
    {
      for(int col = 0; col < end_col; col++)
        {
          int distance = sqrt(pow((xCenter - col), 2)
                              + pow ((yCenter - row), 2));
          printf("distance: %d, radius: %d, col: %d, row: %d\n", distance, radius, col, row);
          if (distance <= radius)
            {
              Pixel * pix = &((*pic).pix_array[row][col]);
              int red = (*pix).R;
              int green = (*pix).G;
              int blue = (*pix).B;
              int grey_scale = (0.30 * red) + (0.59 * green) + (0.11 * blue);
              (*pix).R = grey_scale;
              (*pix).G = grey_scale;
              (*pix).B = grey_scale;
            }
        }
    }
}




//This helper function calculates the brightness of a pixel.
int
brightness (Pixel pix)
{
  return pix.R + pix.B + pix.G;
}

//Makes an array of the brightness of each pixel of the original picture
void
make_array(int end_row, int end_col, int arr_b[][end_col], const Picture * pic)
{
    for(int row = 0; row < end_row; row++)
    {
      for(int col = 0; col < end_col; col++)
        {
          arr_b[row][col] = brightness((*pic).pix_array[row][col]);
        }
    }

}

//This helper function calculates the directional strength of a pixel.
int
edge_strength (int row, int col, int end_col, int arr_b[][end_col])
{
  int a = arr_b[row-1][col-1];
  int b = arr_b[row-1][col];
  int c = arr_b[row-1][col+1];
  int d = arr_b[row][col-1];
  int f = arr_b[row][col+1];
  int g = arr_b[row+1][col-1];
  int h = arr_b[row+1][col];
  int i = arr_b[row+1][col+1];
  
  int horizontal_strength = (a + 2*d +g) - (c + 2*f + i);
  int vertical_strength = (a + 2*b +c) - (g + 2*h + i);
  int delta_strength = sqrt((pow(horizontal_strength,2) + pow(vertical_strength,2)));
  return delta_strength * 255 / 1140;
}


// Returns a new picture that reflects the change in brightness in the original picture.
Picture
pictureEdges (const Picture * pic)
{
  Picture edge_pic;
  edge_pic.height = (*pic).height;
  edge_pic.width = (*pic).width;
  
  int end_row = (*pic).height;
  int end_col = (*pic).width;

  int arr_b[end_row][end_col];
  make_array(end_row, end_col, arr_b, pic);
  
  for(int row = 0; row < end_row; row++)
    {
      for(int col = 0; col < end_col; col++)
        {
          Pixel * pix = &(edge_pic.pix_array[row][col]);
          if (row == 0 || row == (end_row - 1) ||
              col == 0 || col == (end_col -1))
            {
              (*pix).R = 0;
              (*pix).G = 0;
              (*pix).B = 0;
            }
          else
            {
              int grey_scale = edge_strength(row, col, end_col, arr_b);
              printf("grey_scale: %d\n", arr_b[row][col]);
              (*pix).R = grey_scale;
              (*pix).G = grey_scale;
              (*pix).B = grey_scale;
            }
        }
    }
  return edge_pic;        
}

//Main executive program, testing.
int
main (void)
{
  rConnect ("/dev/rfcomm0");
  Picture pic1 = rTakePicture();
  rSavePicture (&pic1, "Original1.jpeg");
  pixelStrip(&pic1, 'R');
  rSavePicture (&pic1, "OriginalStripR.jpeg");

  Picture pic2 = rTakePicture();
  pixelStrip(&pic2, 'G');
  rSavePicture (&pic2, "OriginalStripG.jpeg");

  Picture pic3 = rTakePicture();
  pixelStrip(&pic3, 'B');
  rSavePicture (&pic3, "OriginalStripB.jpeg");

  Picture pic4 = rTakePicture();
  pictureRedder(&pic4);
  rSavePicture (&pic4, "OriginalRedder.jpeg");

  Picture pic5 = rTakePicture();
  pictureGreener(&pic5);
  rSavePicture (&pic5, "OriginalGreener.jpeg");

  Picture pic6 = rTakePicture();
  pictureBluer (&pic6);
  rSavePicture (&pic6, "OriginalBluer.jpeg");

  Picture pic7 = rTakePicture();
  circleSelect(&pic7, 50, 50, 50);
  rSavePicture (&pic7, "OriginalSelect.jpeg");

  Picture pic8 = rTakePicture();
  Picture newpic = pictureEdges(&pic8);
  rSavePicture (&newpic, "OriginalContrast.jpeg");
    
  rDisconnect();
  return 0;
  
}

