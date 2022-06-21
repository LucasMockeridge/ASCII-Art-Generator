#include <iostream>
#include <iomanip>
#include "Image.hh"

using std::cerr, std::cout, std::setw;

int main( int argc, char* argv[] )
{
	if ( argc == 1 )
	{
		cerr << "usage: ./main image_path [ansi_code]\n" << setw( 49 ) << "image_path: path to a 24-bit bitmap image\n" << setw( 46 ) << "ansi_code: ansi code between 0 and 255\n";
		return 1;
	}
	else if ( argc > 2 )
	{
		cout << "\033[" << argv[2] << "m";
	}
	Image image;
	image.read(argv[1]);
	image.write();
	cout << "\033[0m";
	return 0;
}
