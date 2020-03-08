#include <SDL_mixer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct snode
{
    char *Path;
    char *Name;
    char *Artist;
    struct snode * next;
    struct snode *prev;
}Snode;

typedef struct song_list
{
    Snode *head;
    Snode *selected;
    int number_of_songs;
}Slist;

Slist * initialize(); //initializes members of the the list
Snode * create_node(char Path[],char Artist[]); //creates the new node
void insert_beg(Slist * list,char * Path,char * Artist);//inserts node at the beginning
void insert_end(Slist *list, char *Path, char *Artist);//inserts node at the end
void insert_at(Slist *list, int pos,char *Path,char * Artist);//inserts node at given position
void show_playlist(Slist *list); //Displays the play-list

int delete_at(Slist *list,int pos); //deletes node at the given position
int delete_end(Slist *list);//deletes node at end
int delete_beg(Slist * list);//deletes the node at the beginning

int delete_song(Slist *list,int flag); // chooses which delete function to call
void insert_song(Slist *list,int flag); //chooses which insert function to call

int search_artist_single(Slist *list);
int search_artist_all(Slist *list);
int search_name(Slist *list);

int next_song(Slist *list);
int prev_song(Slist *list);
int get_selected(Slist *list);
