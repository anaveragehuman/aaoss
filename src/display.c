#include <stdio.h>

#include "aaoss.h"
#include "disk.h"

static void show_die(const char *op) {
  fprintf(stderr, "Unknown argument %s to S command.\n", op);
}

static void show_processes(const struct process *const *pcb) {
  if (*pcb) {
    printf("  %6s %6s %2c\n", "PID", "PR", 'S');
    process_show(*pcb, 'R');

    struct process *it = (*pcb)->next;
    while (it) {
      process_show(it, 'S');
      it = it->next;
    }
  }
}

static void show_disks(const struct process *const *disks, const size_t ndisk) {
  printf("  %6s %2c %2s\n", "PID", 'S', "NAME");
  for (size_t i = 0; i < ndisk; i++) {
    disk_show(disks[i], i);
  }
}

void show(const struct process *const *pcb, const struct process *const *disks,
          const size_t numdisks, const char *op) {
  if (op[1]) { // more than one character argument, abort
    show_die(op);
    return;
  }

  switch (*op) {
  case 'r':
    show_processes(pcb);
    break;
  case 'i':
    show_disks(disks, numdisks);
    break;
  case 'm':
    break;
  default:
    show_die(op);
  }
}
