#define PM __attribute__((pointer_type(1)))

void *pm_root();

struct test {
  int a;
};

int main() {
  int *array[1] = {0};
  PM int *ptr = array[0];
}