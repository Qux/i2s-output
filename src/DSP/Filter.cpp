#include "Filter.h"

Filter::Filter(Filtertype _filtertype) {
    windowsize = MAX_WINDOW_SIZE;
    filtertype = _filtertype;
}

void Filter::init() {
    for (int i = 0; i < MAX_WINDOW_SIZE; i++) { // TODO dlineの定義部分と同じ値にしたい。ハードコードをやめたい。
        dline.add(0.0);
    }
    index = 0;
    initial_index = 0;
    sum = 0.0;
}

void Filter::setFiltertype(const Filtertype _filtertype) {
    filtertype = _filtertype;
}

void Filter::setWindowsize(const int _windowsize) {
    windowsize = _windowsize; // TODO: not available because of the hard-coded arraysize.
}

void Filter::setIndex(const std::size_t _index) {
    index = _index;
}

float Filter::process(float in) { 
    float out = 0.0;
    switch(filtertype) {
        case LowPass:
            dline.add(in);
            sum += dline.get(0); // inと同じもののつもり
            if (initial_index < windowsize) {
                initial_index++;
                out = sum / (initial_index + 1);
            } else {
                sum -= dline.get(windowsize - 1);
                out = sum / windowsize;
            }
            break;

        case HighPass:
            break;
        case AllPass:
            dline.add(in);
            out = dline.get(index);// TODO Delayline.hのz0_indexは0で初期化が必要？
            break;
        default:
            break;
    }
    index = (index + 1) % MAX_WINDOW_SIZE;
    return out;
}

StereoSample Filter::process(StereoSample sample) {
    StereoSample out;
    out.L = Filter::process(sample.L);
    out.R = Filter::process(sample.R);
    return out;
}