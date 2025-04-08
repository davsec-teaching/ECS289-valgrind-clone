// valgring_clone.c

unsigned long heap_count;

void incr_and_print_count() {
  heap_count++;
  printf("Current heap allocations: %d\n", heap_count);
}

void decr_and_print_count() {
  heap_count--;
  printf("Current heap allocations: %d\n", heap_count);
}
