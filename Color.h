#pragma once

namespace Constellation {

    class Color
    {
    public:
        enum Representation { rgb, hsl, hsv };

    private:
        Representation representation;
        char alpha;
        char red = 0;
        char green = 0;
        char blue = 0;
        char hue = 0;
        char saturation = 0;
        char lightness = 0;
        char value = 0;

        Color(Representation representation, char alpha) :
            representation(representation), alpha(alpha) {};


    public:
        static Color NewRGB(char red, char green, char blue, char alpha);
        static Color NewHSL(char hue, char saturation, char lightness, char alpha);
        static Color NewHSV(char hue, char saturation, char value, char alpha);
    };

}
