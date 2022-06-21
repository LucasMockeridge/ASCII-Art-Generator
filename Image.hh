class Image
{
	public:
		void read( const char* filepath );
		void write();
	private:
		int width;
		int height;
		unsigned char* pixels;
		unsigned char toGreyscale( const unsigned char* pixel );
		unsigned char gamma( double value ); 
		double inverseGamma( const unsigned char value) ;
};
