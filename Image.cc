#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdlib>
#include "Image.hh"

using std::ifstream, std::ios, std::cerr, std::exit, std::string, std::cout;

const int BITMAPFILEHEADER_SIZE = 14;
const int BITMAPINFOHEADER_SIZE = 40;

void
Image::read( const char* filepath )
{
	ifstream f;
	f.open( filepath, ios::binary );

	if( !f.is_open() )
	{
		cerr << "Can't open: " << filepath << "!\n";
		exit( 1 );
	}

	unsigned char file_header[BITMAPFILEHEADER_SIZE];
	unsigned char info_header[BITMAPINFOHEADER_SIZE];

	f.read( reinterpret_cast<char*>( file_header ), BITMAPFILEHEADER_SIZE );

	if ( static_cast<char>( file_header[0] ) != 'B' || static_cast<char>( file_header[1] ) != 'M' )
	{
		cerr << "Invalid BMP!" << "\n";
		exit( 1 );
	}

	f.read( reinterpret_cast<char*>( info_header ), BITMAPINFOHEADER_SIZE );

	width = info_header[4] + ( info_header[5] << 8 ) + ( info_header[6] << 16 ) + ( info_header[7] << 24 );
	height = info_header[8] + ( info_header[9] << 8 ) + ( info_header[10] << 16 ) + ( info_header[11] << 24 );

	const int padding = width % 4;

	const unsigned char bits_pp = info_header[14] + ( info_header[15] << 8 );
	if ( bits_pp != 24 )
	{
		cerr << "Not 24-bit Bitmap!" << "\n";
		exit( 1 );
	}

	const unsigned int comp = info_header[16] + ( info_header[17] << 8 ) + ( info_header[18] << 16 ) + ( info_header[19] << 24 );
	if ( comp != 0 )
	{
		cerr << "Compressed Bitmap!" << "\n";
		exit( 1 );
	}

	bool invert = true;
	if ( height < 0 )
	{
		invert = false;
		height = -height;
	}

	pixels = new unsigned char[width * height];
	for ( int i = 0; i < height; i++ )
	{
		for ( int j = 0; j < width; j++ )
		{
			unsigned char pixel[3];
			f.read( reinterpret_cast<char*>( pixel ), 3 );
			const unsigned char grey_value = toGreyscale( pixel );
			int n = invert ? height - 1 - i : i;
			pixels[n * width + j] = grey_value;
		}
			f.ignore( padding );
	}
	f.close();
}

void
Image::write()
{
	string ascii = "`^\",:;Il!i~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
	for ( int i = 0; i < height; i++ )
	{
		for ( int j = 0; j < width; j++ )
		{
			unsigned char pixel = pixels[i * width + j];
			for ( int k = 0; k < 3; k++ )
			{
				cout << ascii[static_cast<int>( pixel / 255.0 * ( ascii.length() - 1 ) )];
			}
		}
		cout << "\n";
	}
	delete [] pixels;	
}

unsigned char
Image::toGreyscale( const unsigned char* pixel )
{
	return gamma( 0.212655 * inverseGamma( pixel[2] ) + 0.715158 * inverseGamma( pixel[1] ) + 0.072187 * inverseGamma( pixel[0] ));
}

unsigned char
Image::gamma( double value )
{
	if ( value <= 0.0031308 )
	{
		value *= 12.92;
	}
	else
	{
		value = 1.055 * pow( value, 1.0 / 2.4 ) - 0.055;
	}
	return static_cast<unsigned char>(value * 255 + 0.5);
}

double
Image::inverseGamma( const unsigned char value )
{
	const double n = value / 255.0;
	if ( n <= 0.04045 )
	{
		return n / 12.92;
	}
	return pow( ( n + 0.055 ) / 1.055, 2.4 );
}
