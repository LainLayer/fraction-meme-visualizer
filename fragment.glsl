#version 330 core

layout(location = 0) out vec4 diffuseColor;

uniform int offset_x;
uniform int offset_y;

int count_digit_in_number(int number, int count_me) {
    int count = 0;
    while (number != 0) {
        int digit = number % 10;
        number /= 10;

        if (digit == count_me) count += 1;
    }

    return count;
}

int remove_digit_from_number(int number, int remove_me, int which_index) {
    int factor     = 1;
    int result     = 0;
    int count_seen = 0;

    while (number != 0) {
        int digit = number % 10;
        number /= 10;
        if ((digit != remove_me) || (digit == remove_me && count_seen != which_index)) {
            result += digit * factor;
            factor *= 10;
        } else {
            count_seen += 1;
        }
    }

    return result;
}

bool is_fraction_good(int top, int bottom) {
    if (bottom == 0)
        return false;

    float expected_result = float(top) / float(bottom);

    int top_copy = top;
    while (top_copy != 0) {
        int digit = top_copy % 10;
        top_copy /= 10;

        int amount_in_bottom = count_digit_in_number(bottom, digit);
        int amount_in_top    = count_digit_in_number(top, digit);

        for (int i = 0; i < amount_in_bottom; i++) {
            for (int j = 0; j < amount_in_top; j++) {
                int new_bottom = remove_digit_from_number(bottom, digit, i);
                if (new_bottom == 0) continue;

                int new_top = remove_digit_from_number(top, digit, j);

                float new_fraction = float(new_top) / float(new_bottom);
                if (abs(new_fraction - expected_result) < 0.0001) return true;
            }
        }
    }

    return false;
}

void main() {

    vec2 position = gl_FragCoord.xy;
    bool is_good = is_fraction_good(int(position.y) + offset_y, int(position.x) + offset_x);

    if (is_good)
        diffuseColor = vec4(0.8, 0.9, 0.7, 1.0);
    else
        diffuseColor = vec4(0.05, 0.05, 0.1, 1.0);
}

