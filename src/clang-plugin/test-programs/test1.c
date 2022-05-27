#define PM_PTR

void *pm_root();

__attribute__((pointer_type (0))) int test1;

void test(int *a, int *b) {
  int *c = a;
  int *d = b;
}
int main() {
  int *ptr = (int *)pm_root();
  int *c = 0;
  test(ptr, c);
}