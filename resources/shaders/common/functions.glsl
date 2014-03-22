/// Возвращает яркость пикселя
float luma( vec3 color ) {
	return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}