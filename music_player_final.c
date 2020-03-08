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

int next_song(Slist *list)
{
    if(list->head)
    {
        if(list->selected->next)
            list->selected = list->selected->next;
        else
            list->selected = list->head;
    }
    return 1;
}

int prev_song(Slist *list)
{
    if(list->head)
    {
        if(list->selected->prev)
            list->selected = list->selected->prev;
        else
        {
            while(list->selected->next)
                list->selected = list->selected->next;
        }
    }
    return 1;
}

int get_selected(Slist *list)
{
    if(list->head)
    {
        int pos = 0;
        Snode * trav = list->head;
        while(trav!=list->selected)
        {
            pos++;
            trav = trav->next;
        }
        printf("The currently selected song is...\n");
        printf("Song Name: %s\n",list->selected->Name);
        printf("Artist Name: %s\n",list->selected->Artist);
        printf("Absolute Path: %s\n",list->selected->Path);
        printf("Position in Play-List: %d\n",pos);
        return pos;
    }
    printf("Sorry play-list is empty!\n");
    return -1;
}

int main()
{
    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) < 0)
    {
        printf("SDL_Mixer Error: %s\n",SDL_GetError());
    }
    else
    {
        Mix_Music * song = NULL;
        Slist *mylist = initialize();
        int i;
        char choice = 'h';
        int changed = 0;
        printf("*************************\n");
        printf("MENU:\n");
        printf("i...Insert song\n");
        printf("e...Delete\\Erase song\n");
        printf("d...Search and Display\n");
        printf("p...Play music\n");
        printf("s...Stop music\n");
        printf("r...Rewind song\n");
        printf("f...Go to next song\n");
        printf("b...Go to previous song\n");
        printf("c...Display currently selected song\n");
        printf("l...Look by name/artist\n");
        printf("h...Display menu \n");
        printf("x...Exit\n");
        printf("*************************\n");
        while(choice!='x')
        {
            printf("choice>> ");
            choice = getch();
            printf("%c\n",choice);
            switch(choice)
            {
                case 'i':   printf("1...Insert song at the beginning\n");
                            printf("2...Insert song at a position\n");
                            printf("3...Insert song at the end\n");
                            printf("choice>> ");
                            choice = getch();
                            printf("%c\n",choice);
                            switch(choice)
                            {
                                case '1':   insert_song(mylist,0);
                                            break;
                                case '2':   insert_song(mylist,1);
                                            break;
                                case '3':   insert_song(mylist,2);
                                            break;
                                default:    printf("Sorry,invalid input\n");
                            }
                            break;

                case 'd':   printf("1...display entire table\n");
                            printf("2...search by name\n");
                            printf("3...search one by artist\n");
                            printf("4...search all by artist\n");
                            printf("choice>> ");
                            choice = getch();
                            printf("%c\n",choice);
                            switch(choice)
                            {
                                case '1':   show_playlist(mylist);
                                            break;
                                case '2':   search_name(mylist);
                                            break;
                                case '3':   search_artist_single(mylist);
                                            break;
                                case '4':   search_artist_all(mylist);
                                            break;
                                default:    printf("Sorry,invalid input\n");
                            }
                            break;
                            show_playlist(mylist);
                            break;

                case 'e':   printf("1...Delete song at the beginning\n");
                            printf("2...Delete song at a position\n");
                            printf("3...Delete song at the end\n");
                            printf("choice>> ");
                            choice = getch();
                            switch(choice)
                            {
                                case '1':   changed = delete_song(mylist,0);
                                            break;
                                case '2':   changed = delete_song(mylist,1);
                                            break;
                                case '3':   changed = delete_song(mylist,2);
                                            break;
                                default:    printf("Sorry,invalid input");
                            }
                            break;

                case 'p':   printf("1...play currently selected song once\n");
                            printf("2...play full play-list once\n");
                            printf("3...play currently selected song on loop\n");
                            printf("2...play full play-list on loop\n");
                            printf("choice>> ");
                            choice = getch();
                            printf("%c\n",choice);
                            switch(choice)
                            {
                                case '1':   if(changed || Mix_PlayingMusic() == 0)
                                            {
                                                if (mylist->selected == NULL)
                                                {
                                                    printf("Sorry,Playlist is Empty\n");
                                                    break;
                                                }
                                                song = Mix_LoadMUS(mylist->selected->Path);
                                                if(song == NULL)
                                                {
                                                    printf("Couldn't load song\n");
                                                }
                                                Mix_PlayMusic(song,1);
                                                if(changed)
                                                    changed = 0;
                                            }
                                            else if(Mix_PausedMusic() == 1)
                                            {
                                                Mix_ResumeMusic();
                                            }
                                            break;

                                case '2':   i = mylist->number_of_songs;
                                            printf("Now playing...\n");
                                            get_selected(mylist);
                                            Mix_PlayMusic(song,1);
                                            while(i && choice == '2')
                                            {
                                                if(song && !Mix_PlayingMusic())
                                                {
                                                    Mix_PlayMusic(song,1);
                                                }
                                                else
                                                {
                                                    changed = next_song(mylist);
                                                    song = Mix_LoadMUS(mylist->selected->Path);
                                                    if (song == NULL)
                                                    {
                                                        printf("Couldn't load song %s",mylist->selected->Name);
                                                    }
                                                    else
                                                    {   printf("Now playing...\n");
                                                        get_selected(mylist);
                                                        Mix_PlayMusic(song,1);
                                                    }
                                                    i--;
                                                }
                                            }
                                            break;
                                case '3':   break;
                                case '4':   break;
                            }
                            break;

                case 'f':   changed = next_song(mylist);
                            Mix_PauseMusic();
                            get_selected(mylist);
                            break;

                case 'b':   changed = prev_song(mylist);
                            Mix_PauseMusic();
                            get_selected(mylist);
                            break;

                case 'c':   get_selected(mylist);
                            break;

                case 's':   {
                                if(Mix_PlayingMusic() == 1)
                                    Mix_PauseMusic();
                            }
                            break;
                case 'r':
                            {

                                Mix_PlayMusic(song,1);
                                Mix_PauseMusic();
                            }
                            break;

                case 'h':   printf("*************************\n");
                            printf("MENU:\n");
                            printf("i...Insert song\n");
                            printf("e...Delete\\Erase song\n");
                            printf("d...Search and Display\n");
                            printf("p...Play music\n");
                            printf("s...Stop music\n");
                            printf("r...Rewind song\n");
                            printf("f...Go to next song\n");
                            printf("b...Go to previous song\n");
                            printf("c...Display currently selected song\n");
                            printf("l...Look by name/artist\n");
                            printf("h...Display menu \n");
                            printf("x...Exit\n");
                            printf("*************************\n");
                            break;

                case 'x':   Mix_CloseAudio();
                            Mix_FreeMusic(song);
                            break;
                default:;
            }
        }
    }

}

Slist * initialize()
{
    Slist * temp = (Slist *)malloc(sizeof(Slist));
    temp->head = NULL;
    temp->selected = NULL;
    temp->number_of_songs = 0;
    return temp;
}

Snode * create_node(char Path[],char Artist[])
{
    Snode * temp = (Snode *)malloc(sizeof(Snode));
    int i,pos = 0;

    int len = strlen(Artist);
    temp->Artist = (char *)malloc(sizeof(char)*(len+1));
    for(i=0;i<=len;i++)
    {
        temp->Artist[i] = Artist[i];
    }

    len  = strlen(Path);
    temp->Path = (char *)malloc(sizeof(char)*(len+1));
    for(i=0;i<=len;i++)
    {
        if(Path[i] == '\\')
        {
            temp->Path[i] = '/';
        }
        else
            temp->Path[i] = Path[i];
    }

    for(i=0;i<len;i++)
    {
        if(temp->Path[i] == '/')
            pos = i;
    }
    temp->Name = (char *)malloc(sizeof(char)*(len-pos));
    for(i=0;Path[pos+1+i]!='.' && (pos+1+i)<len;i++)
        temp->Name[i] = Path[pos+1+i];
    for(i=i;i<len-pos;i++)
        temp->Name[i] = '\0';
    temp->next = NULL;
    temp->prev = NULL;
}

void insert_beg(Slist * list,char * Path,char * Artist)
{
    Snode *temp = create_node(Path,Artist);
    if(list->head)
        list->head->prev = temp;
    else
        list->selected = temp;
    temp->next = list->head;
    list->head = temp;
    list->number_of_songs++;
}

void insert_end(Slist *list, char *Path, char *Artist)
{
    Snode *temp = create_node(Path,Artist);
    Snode *trav = list->head;
    if(list->head == NULL)
    {
        list->head = temp;
        list->selected = temp;
    }
    else
    {
        while(trav->next)
            trav = trav->next;
        trav->next = temp;
        temp->prev = trav;
    }
    list->number_of_songs++;
}

void insert_at(Slist *list, int pos,char *Path,char * Artist)
{
    if(pos>=0 && pos <= list->number_of_songs)
    {
        if(pos == 0)
            insert_beg(list,Path,Artist);
        else
        {
            pos--;
            Snode * temp = create_node(Path,Artist);
            Snode * trav = list->head;
            while(pos)
            {
                trav = trav->next;
                pos--;
            }
            temp->next = trav->next;
            temp->prev = trav;
            if(trav->next)
                trav->next->prev = temp;
            trav->next = temp;
            list->number_of_songs++;
        }
    }
    else
        printf("Invalid position entered\n");
}

void show_playlist(Slist *list)
{
    int i;
    if(list->head == NULL)
    {
        printf("Sorry, The play-list is empty!\n");
    }
    else
    {
        Snode *trav = list->head;
        int i = 0;
        while(trav)
        {
            printf("%d\t%s\t%s\t%s\n",i,trav->Path,trav->Name,trav->Artist);
            i++;
            trav = trav->next;
        }
    }
}

int delete_at(Slist *list,int pos)
{
    int changed = 0;
    if(list->head == NULL)
    {
        printf("The play-list is empty\n");
    }
    else if(pos>=0 && pos<list->number_of_songs)
    {
        if(pos == 0)
        {
            delete_beg(list);
        }
        else
        {
            pos--;
            Snode * trav = list->head;
            while(pos)
            {
                trav = trav->next;
            }

            if(list->selected == trav->next)
            {
                changed = 1;
                list->selected = trav;
            }
            Snode * temp = trav->next;
            if(trav->next->next)
                trav->next->next->prev = trav;
            trav->next = trav->next->next;
            free(temp);

            list->number_of_songs--;
        }
    }
    else
        printf("Invalid position entered\n");
    return changed;
}

int delete_beg(Slist * list)
{
    int changed = 0;
    if(list->head == NULL)
    {
        printf("The play-list is empty\n");
    }
    else
    {
        Snode * temp = list->head;
        if(list->selected == list->head)
        {
            changed = 1;
            list->selected = list->head->next;
        }
        list->head = list->head->next;
        if(list->head)
            list->head->prev = NULL;
        else
            list->selected = NULL;
        free(temp);
        temp = NULL;
        list->number_of_songs--;
    }
    return changed;
}

int delete_end(Slist *list)
{
    int changed = 0;
    if(list->head == NULL)
    {
        printf("The play-list is empty\n");
    }
    else
    {
        Snode * trav = list->head;
        while(trav->next)
        {
            trav = trav->next;
        }

        if(list->selected == trav)
        {
            list->selected = trav->prev;
            changed = 1;
        }
        if(trav->prev)
            trav->prev->next = NULL;
        else
            list->head = NULL;
        free(trav);
        trav = NULL;
        list->number_of_songs--;
    }
    return changed;
}

void insert_song(Slist *list,int flag)
{
    char Path[1000];
    char Artist[100];
    int pos;
    printf("Enter the absolute path of the song: ");
    scanf("%s",Path);
    printf("Enter the name of the artist: ");
    scanf("%s",Artist);
    if(flag == 0)
        insert_beg(list,Path,Artist);
    else if(flag == 1)
    {
        printf("Enter the position of the song in the playlist: ");
        scanf("%d",&pos);
        insert_at(list,pos,Path,Artist);
    }
    else if(flag == 2)
    {
        insert_end(list,Path,Artist);
    }
}

int delete_song(Slist *list,int flag)
{
    int pos;
    if(flag == 0)
        return delete_beg(list);
    else if(flag == 1)
    {
        printf("Enter the position of the song in the playlist: ");
        scanf("%d",&pos);
        return delete_at(list,pos);
    }
    else if(flag == 2)
    {
        return delete_end(list);
    }
}

int search_artist_single(Slist *list)
{
    char Artist[100];
    printf("Enter Artist: ");
    scanf("%s",Artist);
    if(list->head)
    {
        int pos = 0;
        Snode * trav = list->head;
        while(trav)
        {
            if(!strcmp(Artist,trav->Artist))
            {
                printf("Song found at position %d\n",pos);
                return pos;
            }
            trav = trav->next;
            pos++;
        }
    }
    return -1;
}

int search_artist_all(Slist *list)
{
    char Artist[100];
    printf("Enter Artist: ");
    scanf("%s",Artist);
    printf("Searching for all songs by %s...\n",Artist);
    int pos = 0;
    if(list->head)
    {
        Snode * trav = list->head;
        while(trav)
        {
            if(!strcmp(Artist,trav->Artist))
            {
                printf("%d\t%s\t%s\n",pos,trav->Name,trav->Artist);
                pos++;
            }
            trav = trav->next;
        }
    }
    if(pos == 0)
        printf("Sorry no songs found\n");
}

int search_name(Slist *list)
{
    char Name[100];
    printf("Enter name of song: ");
    scanf("%s",Name);
    if(list->head)
    {
        Snode * trav = list->head;
        int pos = 0;
        while(trav)
        {
            if(!strcmp(Name,trav->Name))
            {
                return pos;
            }
            pos++;
            trav = trav->next;
        }
    }
    return -1;
}

