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
    printf("Sorry, song not found!\n");
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
                printf("Song found at position %d\n",pos);
                return pos;
            }
            pos++;
            trav = trav->next;
        }
    }
    printf("Sorry, song not found!\n");
    return -1;
}

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
