
const float PI =		3.1415926535898;
const float TWO_PI =	6.2831853071795;
const float E = 		2.7182818284590;

/// Возвращает яркость пикселя
float luma( vec3 color ) {
	return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}