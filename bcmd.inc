#include <similar_text>

#define words::%0(%1) \
    _words_lib_%0(%1)

#define _MAX_BAD_WORDS  10
#define _MAX_WORD_LEN   16
#define _MIN_PERCENTAGE 5.0

new
    _bad_words[_MAX_BAD_WORDS][_MAX_WORD_LEN]
    //variable that stores all words located in badwords.txt

;



public OnGameModeInit()
{
    //hook
    #if defined _word_lib_OnGameModeInit
        _word_lib_OnGameModeInit();
    #endif
    //end of hook

    _words_lib_get_words(); //function to get all words from badwords.txt
    
    return 1;
}
//hook
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit _word_lib_OnGameModeInit
#if defined _word_lib_OnGameModeInit
    forward _word_lib_OnGameModeInit();
#endif
//end of hook

public OnPlayerText(playerid,text[])
{
    //hook
    #if defined _word_lib_OnPlayerText
        _word_lib_OnPlayerText(playerid,text);
    #endif
    //end of hook

    //we do a loop to check all words from _bad_words[i]
    for(new i; i < _MAX_BAD_WORDS; i++)
    {
        //check if the percentage from similar_text is > than _MIN_PERCENTAGE (5.0)
        if(similar_text(text,_bad_words[i]) > _MIN_PERCENTAGE)
        {
            //check if the text contais a _bad_words
            if(strfind(text,_bad_words[i]) != -1)
            {
                //if the text contains a _bad_word the averange will be 100.00
                CallLocalFunction("OnPlayerSayBadWord","dssf",playerid,text,_bad_words[i],100.0);
                return false;
                //returning false to avoid bugs
            }
            else
                CallLocalFunction("OnPlayerSayBadWord","dssf",playerid,text,_bad_words[i],similar_text(text,_bad_words[i]));
            if(i == (_MAX_BAD_WORDS-1))
                return false;
            //the text will not be sent to the server.
        }
    }
    
    return 1;
}
//hook
#if defined _ALS_OnPlayerText
    #undef OnPlayerText
#else
    #define _ALS_OnPlayerText
#endif
#define OnPlayerText _word_lib_OnPlayerText
#if defined _word_lib_OnPlayerText
    forward _word_lib_OnPlayerText(playerid,text[]);
#endif
//end of hook


/*
    functions
*/

stock words::get_words()
{
    //check if the file exists
    if(fexist("badwords.txt"))
    {
        //if yes load the file
        new File:words = fopen("badwords.txt",io_read);
        for(new i; i < _MAX_BAD_WORDS; i++)
        {
            fread(words,_bad_words[i]);
            if( i != (_MAX_BAD_WORDS - 1 ))
                _bad_words[i][strlen(_bad_words[i]) - 2] = EOS;
            printf("str: %s , len: %d",_bad_words[i], strlen(_bad_words[i]));
        }
        fclose(words);
        return true;
    }
    //if not create a file
    else
    {
        new File:words = fopen("badwords.txt",io_append);
        if(words)
        {
            fclose(words);
        }
        print("Aviso: O ficheiro badwords.txt foi criado, é necessário colocar palavras!");
        return false;
    }
}

/*
    called functions
*/

forward OnPlayerSayBadWord(playerid,text[],badword[],Float:averange);
/*
    playerid - the id of the player
    text - the text that the player will sent
    badword - the badword that the player could say
    averange - the averange between the badword and the text , returned by similar_text or strfind check
*/
