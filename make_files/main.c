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
        char choice = 'h';
        int changed = 0;
        printf("*************************\n");
        printf("MENU:\n");
        printf("i...Insert song\n");
        printf("e...Delete\\ Erase song\n");
        printf("d...Search and Display\n");
        printf("p...Play music\n");
        printf("s...Stop music\n");
        printf("r...Rewind song\n");
        printf("f...Go to next song\n");
        printf("b...Go to previous song\n");
        printf("c...Display currently selected song\n");
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

                case 'p':   if(changed || Mix_PlayingMusic() == 0)
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
                            printf("e...Delete\\ Erase song\n");
                            printf("d...Search and Display\n");
                            printf("p...Play music\n");
                            printf("s...Stop music\n");
                            printf("r...Rewind song\n");
                            printf("f...Go to next song\n");
                            printf("b...Go to previous song\n");
                            printf("c...Display currently selected song\n");
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
