#include <darcs_patch.h>

int main() {
  printf("hi\n");
  
  printf("\n\n\nhttp://darcs.net/Using/Model#hunk-patches\n\n");
  
  DarcsPatch::Patcher_T hunks;
  hunks.add(1, "", "all\nwere\noccupied");
  //hunks.add(2, "were", "the\nseats\nwere");
  // also valid
  //hunks.add(2, "", "the\nseats");
  hunks.add(2, "", "clean");
  hunks.add(4, "", "duly");
  //hunks.add(5, "", "blue");
  //hunks.add(6, "seats", "tables");
  
  hunks.calculateDeps();
  
  /*
  
  hunks = {};
  hunks.add(0, "", "Hello World");
  
  // removal
  // hunks.add(0, Hello World", "");
  
  hunks.add(0, "Hello World", "Hello \n World");
  
  hunks.calculateDeps();
  
  // removal
  // hunks.add(0, "Hello \n World", "Hello World");
  */
  return 0;
}
