#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disk.h"
#include "process.h"

struct process **disks_create(const size_t numdisks) {
  struct process **disks;
  size_t memsize = numdisks * sizeof *disks;
  disks = malloc(memsize);
  return memset(disks, 0, memsize);
}

void disks_destroy(struct process **disks, const size_t numdisks) {
  for (size_t i = 0; i < numdisks; i++) {
    while (disks[i]) {
      process_exit(&disks[i]);
    }
  }

  free(disks);
}

void disk_request(struct process **pcb, struct process **disks, long int disk,
                  const char *filename) {
  struct process *proc = process_dequeue(pcb);

  // copy filename as it'll be freed by the main loop
  size_t memsize = sizeof *(proc->filename) * strlen(filename) + 1; // 1 for NUL
  proc->filename = realloc(proc->filename, memsize);
  strcpy(proc->filename, filename);

  struct process *tmp = disks[disk];
  if (tmp) {
    while (tmp->next) {
      tmp = tmp->next;
    }

    tmp->next = proc;
  } else {
    disks[disk] = proc;
  }

  proc->prev = tmp;
}

void disk_done(struct process **pcb, struct process **disks, long int disk) {
  struct process *proc = process_dequeue(&disks[disk]);
  if (proc) {
    process_insert(pcb, proc);
  }
}

void disk_show(const struct process *disk, const size_t disknum) {
  struct process *tmp = disk;
  if (tmp) {
    printf("Disk %lu:\n", disknum);
    while (tmp) {
      printf("  %6d %2c %2s\n", tmp->pid, 'D', tmp->filename);
      tmp = tmp->next;
    }
  }
}
