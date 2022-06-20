#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdlib>

#define BITMAPFILEHEADER_SIZE 14
#define BITMAPINFOHEADER_SIZE 40

using std::string, std::cout, std::cerr, std::exit;

class Image {
public:
	Image(const char* filepath) {
		read(filepath);
		write();
	}
	
private:
	int width;
	int height;
	unsigned char* pixels;

	void read(const char* filepath) {
		std::ifstream f;
		f.open(filepath, std::ios::binary);

		if (!f.is_open()) {
			cerr << "Can't open: " << filepath << "!\n"; 
			exit(1);
		}

		unsigned char file_header[BITMAPFILEHEADER_SIZE];
		unsigned char info_header[BITMAPINFOHEADER_SIZE];

		f.read(reinterpret_cast<char*>(file_header), BITMAPFILEHEADER_SIZE);

		if (static_cast<char>(file_header[0]) != 'B' || static_cast<char>(file_header[1]) != 'M') {
			cerr << "Invalid BMP!" << "\n";
			exit(1);
        }

		f.read(reinterpret_cast<char*>(info_header), BITMAPINFOHEADER_SIZE);
		
		width = info_header[4] + (info_header[5] << 8) + (info_header[6] << 16) + (info_header[7] << 24);
		height = info_header[8] + (info_header[9] << 8) + (info_header[10] << 16) + (info_header[11] << 24);

		const int padding = width % 4;
	
		const unsigned char bits_pp = info_header[14] + (info_header[15] << 8);
		if (bits_pp != 24) {
			cerr << "Not 24-bit Bitmap!" << "\n";
			exit(1);
		}
		
		const unsigned int comp = info_header[16] + (info_header[17] << 8) + (info_header[18] << 16) + (info_header[19] << 24);
		if (comp != 0) {
			cerr << "Compressed Bitmap!" << "\n";
			exit(1);
		}

		bool invert = true;
		if (height < 0) {
			invert = false;
			height = -height;
		}
		
		pixels = new unsigned char[width * height];
		int n = invert ? height - 1 : 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				unsigned char pixel[3];
				f.read(reinterpret_cast<char*>(pixel), 3);
				const unsigned char grey_value = to_greyscale(pixel);
				pixels[(n - i) * width + j] = grey_value;
			}
			f.ignore(padding);
		}
		f.close();
	} 

	unsigned char to_greyscale(const unsigned char* pixel) {
		return gamma(0.212655 * inverse_gamma(pixel[2]) + 0.715158 * inverse_gamma(pixel[1]) + 0.072187 * inverse_gamma(pixel[0]));
	}

	double inverse_gamma(const unsigned char value) {
		const double n = value / 255.0;
		if (n <= 0.04045) {
			return n / 12.92;
		}
		return pow((n + 0.055 ) / 1.055, 2.4);
	}

	unsigned char gamma(double value) {
		if(value <= 0.0031308) {
      		value *= 12.92;
		}
    	else { 
      		value = 1.055 * pow(value, 1.0 / 2.4) - 0.055;
		}
    	return static_cast<unsigned char>(value * 255 + 0.5);
	}

	void write() {
		string ascii = "`^\",:;Il!i~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
                unsigned char pixel = pixels[i * width + j];
				for (int k = 0; k < 3; k++) {
					cout << ascii[static_cast<int>(pixel / 255.0 * (ascii.length() - 1))];
				}
            }
			cout << "\n";
        }
		delete [] pixels;
	}
};

int main(int argc, char *argv[]) {
	if (argc == 1 || string(argv[1]) == "-help") {
		cerr << "usage: ./main image_path [ansi_code]\n\timage_path: path to a 24-bit bitmap image\n\tansi_code: ansi code between 0 and 255\n"; 
		return 1;
	}
	else if (argc > 2) {
		cout << "\033[" << argv[2] << "m";
	}
	Image image(argv[1]);
	cout << "\033[0m";
	return 0;
}
