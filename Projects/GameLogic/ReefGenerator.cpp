#include "ReefGenerator.h"
#include <random>
#include <time.h>

ReefGenerator::ReefGenerator()
{};

std::vector<Point> ReefGenerator::generate_reef(int width_in_tiles, int height_in_tiles, double probability)
{
  srand(time(NULL));
  std::vector<Point> generated_reef;
  if ( probability >= 0.0 )
  {
    //Go through every tile
    for ( int i = 0; i < width_in_tiles; i++ )
    {
      for ( int j = 0; j < height_in_tiles; j++ )
      {
        double r = ( (double)rand() / (double)( RAND_MAX ) );
        //If rand() follows a uniform distribution, we should hit our expected percentage
        if ( r <= probability )
        {
          generated_reef.push_back(Point(i, j));
        }
      }
    }
  }
  return generated_reef;
};