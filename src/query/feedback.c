#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *generate_syntax_mistake_feedback(char *query, char *stop_ptr) {
  int indent = stop_ptr - query;
  int q_len = strlen(query);

  // +4 because aside for original query and space indent,
  // we also need memory for
  // ^, \0 and two \n
  char *feedback = malloc((q_len+indent+4)*sizeof(char));
  if (feedback == NULL) {
    printf("failed to allocate memoru for feedback");
    return NULL;
  }

  strncpy(feedback, query, q_len);

  feedback[q_len] = '\n';

  for (int i = q_len+1; i < q_len+1+indent; i++) {
    feedback[i] = ' '; 
  }

  feedback[q_len+1+indent] = '^';

  feedback[q_len+indent+2] = '\n';

  feedback[q_len+indent+3] = '\0';

  return feedback;
}
