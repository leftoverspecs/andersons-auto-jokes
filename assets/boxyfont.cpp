#include "boxyfont.h"

namespace assets {

namespace {

std::vector<float> init_boxyfont_widths() {
    std::vector<float> result(128, 10.0f);
    result[' '] = 4.0f;
    result['!'] = 5.0f;
    result['"'] = 8.0f;
    result['#'] = 10.0f;
    result['$'] = 8.0f;
    result['%'] = 11.0f;
    result['&'] = 10.0f;
    result['\''] = 5.0f;
    result['('] = 6.0f;
    result[')'] = 6.0f;
    result['*'] = 7.0f;
    result['+'] = 9.0f;
    result[','] = 6.0f;
    result['-'] = 7.0f;
    result['.'] = 5.0f;
    result['/'] = 7.0f;
    result['0'] = 8.0f;
    result['1'] = 5.0f;
    result['2'] = 8.0f;
    result['3'] = 8.0f;
    result['4'] = 8.0f;
    result['5'] = 8.0f;
    result['6'] = 8.0f;
    result['7'] = 8.0f;
    result['8'] = 8.0f;
    result['9'] = 8.0f;
    result[':'] = 5.0f;
    result['<'] = 7.0f;
    result['='] = 7.0f;
    result['>'] = 7.0f;
    result['?'] = 9.0f;
    result['@'] = 9.0f;
    result['A'] = 8.0f;
    result['B'] = 8.0f;
    result['B'] = 8.0f;
    result['C'] = 8.0f;
    result['D'] = 8.0f;
    result['E'] = 8.0f;
    result['F'] = 8.0f;
    result['G'] = 8.0f;
    result['H'] = 8.0f;
    result['I'] = 5.0f;
    result['J'] = 8.0f;
    result['K'] = 8.0f;
    result['L'] = 8.0f;
    result['M'] = 10.0f;
    result['N'] = 9.0f;
    result['O'] = 8.0f;
    result['P'] = 8.0f;
    result['Q'] = 9.0f;
    result['R'] = 8.0f;
    result['S'] = 8.0f;
    result['T'] = 9.0f;
    result['U'] = 8.0f;
    result['V'] = 8.0f;
    result['W'] = 10.0f;
    result['X'] = 8.0f;
    result['Y'] = 9.0f;
    result['Z'] = 8.0f;
    result['['] = 6.0f;
    result['\\'] = 7.0f;
    result[']'] = 6.0f;
    result['^'] = 9.0f;
    result['_'] = 7.0f;
    result['`'] = 6.0f;
    result['a'] = 8.0f;
    result['b'] = 8.0f;
    result['c'] = 7.0f;
    result['d'] = 8.0f;
    result['e'] = 8.0f;
    result['f'] = 7.0f;
    result['g'] = 8.0f;
    result['h'] = 8.0f;
    result['i'] = 6.0f;
    result['j'] = 7.0f;
    result['k'] = 8.0f;
    result['l'] = 6.0f;
    result['m'] = 10.0f;
    result['n'] = 8.0f;
    result['o'] = 8.0f;
    result['p'] = 8.0f;
    result['q'] = 8.0f;
    result['r'] = 8.0f;
    result['s'] = 8.0f;
    result['t'] = 7.0f;
    result['u'] = 8.0f;
    result['v'] = 8.0f;
    result['w'] = 10.0f;
    result['x'] = 8.0f;
    result['y'] = 8.0f;
    result['z'] = 8.0f;
    result['{'] = 6.0f;
    result['|'] = 5.0f;
    result['}'] = 6.0f;
    result['~'] = 10.0f;
    result['\x7f'] = 8.0f;
    for (float &f: result) {
        f /= 10.0f;
    }
    return result;
}

}

const std::vector<float> boxyfont_widths = init_boxyfont_widths();

}
