
struct stringList {
        char *data;
        struct stringList *next;
};

typedef struct stringList stringList;

void SL_add(stringList *list, const char *data);

