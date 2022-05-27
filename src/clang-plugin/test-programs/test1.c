void* pm_root();

void test(int *a, int *b) {
  int *c = a;
  int *d = b;
}
int main() {
  int *ptr = (int*)pm_root();
  int *c = 0;
  test(ptr, c);
}