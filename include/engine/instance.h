#pragma one

template <typename T>
class Instance {
    static virtual std::vector<T> instances = 0;
    static virtual T* getInstance(const char *vert, const char *frag, int id) = 0;

    static virtual T* getInstance(int id) = 0;
};