#include "Color.h"

Color Color::NewRGB(char red, char green, char blue, char alpha) {
	Color c = Color(rgb, alpha);
	c.red = red;
	c.green = green;
	c.blue = blue;
	return c;
}

Color Color::NewHSL(char hue, char saturation, char lightness, char alpha) {
	Color c = Color(hsl, alpha);
	c.hue = hue;
	c.saturation = saturation;
	c.lightness = lightness;
	return c;
}

Color Color::NewHSV(char hue, char saturation, char value, char alpha) {
	Color c = Color(hsv, alpha);
	c.hue = hue;
	c.saturation = saturation;
	c.value = value;
	return c;
}
