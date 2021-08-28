/*

    Zelda Return of the Hylian

    Copyright (C) 2005-2008  Vincent Jouillat

    Please send bugreports with examples or suggestions to www.zeldaroth.fr

*/

#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>

#include <iostream>
#include <ctime>
#include <cstdlib>
#include "Keyboard.h"
#include "Generique.h"

SDL_Surface* init() {             // initialise SDL
    if(SDL_Init(SDL_INIT_VIDEO) == -1) {
        printf("Could not load SDL : %s\n", SDL_GetError());
        exit(-1);
    }
    atexit(SDL_Quit);
    //if(SDL_InitSubSystem(SDL_INIT_AUDIO) == -1) *SOUND = false;

    SDL_WM_SetCaption("Return of the Hylian",NULL);
    SDL_Surface* icon = SDL_LoadBMP("data/images/logos/triforce.ico");
    SDL_SetColorKey(icon,SDL_SRCCOLORKEY,SDL_MapRGB(icon->format,0,0,0));
    SDL_WM_SetIcon(icon,NULL);

    SDL_ShowCursor(SDL_DISABLE);

    return SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
}

int main(int argc, char** argv) {
    if (argc && argv); //pour �viter un warning.....

    std::srand(std::time(NULL));

    SDL_Rect src;
    SDL_Rect dst;
    src.w=640; src.h=480; src.y=0;src.x=0;dst.x=0; dst.y=0;

    SDL_Surface* gpScreen = NULL;

    int mode = 2; //mode=0;

    gpScreen = init();
    SDL_Surface* gpScreen2 = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 32, 0, 0, 0, 0);
    SDL_Surface* gpScreen3 = NULL;

    Audio* gpAudio = new Audio();
    Jeu* gpJeu = new Jeu(gpAudio);
    Carte* gpCarte = new Carte(gpJeu);
    Encyclopedie* gpEncyclopedie = new Encyclopedie(gpJeu);
    Keyboard* gpKeyboard = new Keyboard(gpJeu, gpCarte, gpEncyclopedie, gpScreen, mode);
    gpJeu->setKeyboard(gpKeyboard);
    Generique* gpGenerique = new Generique(gpJeu);
    gpJeu->setGenerique(gpGenerique);
    //gpGenerique->initDisclamer();
    gpGenerique->initLogo();

    //gpJeu->init(5);

    bool gLoop = true;

    Uint32 lastAnimTime = SDL_GetTicks();

    while (gLoop) {

        if (gpKeyboard->gererClavier() == -1) {gLoop = false;}

        switch (gpKeyboard->getMode()) {
            case 0 : //jeu normal
                gpJeu->draw(gpScreen2); break;
            case 1 : //disclamer
            case 2 : //logo
            case 3 : //titre
            case 14 : //g�n�rique score
            case 17 : //menu d'aide 1
            case 18 : //menu d'aide 2
                gpGenerique->draw(gpScreen2); break;
            case 4 : //selection
                gpGenerique->drawSelection(gpScreen2, gpKeyboard->getLigne(),
                    gpKeyboard->getColonne()); break;
            case 6 : //options
                gpGenerique->drawOption(gpScreen2, gpKeyboard->getLigneOption(),
                gpKeyboard->getVolume()/8, gpKeyboard->getVolson()/8); break;
            case 7 : //charger partie
                gpGenerique->drawCharger(gpScreen2, gpKeyboard->getLigne(),
                    gpKeyboard->getLigneVal()); break;
            case 8 : //g�n�rique intro
                gpGenerique->drawIntro(gpScreen2, gpKeyboard->getIntro()); break;
            case 9 : //effacer partie
                gpGenerique->drawEffacerSave(gpScreen2, gpKeyboard->getLigne(),
                    gpKeyboard->getLigneVal()); break;
            case 10 : //g�n�rique d�but chez link
                gpGenerique->drawDebut(gpScreen2); break;
            case 11 : //g�n�rique fin
                gpGenerique->drawFin(gpScreen2); break;
            case 12 : //carte
                gpCarte->draw(gpScreen2); break;
            case 13 : //encyclop�die des monstres
                gpEncyclopedie->draw(gpScreen2); break;
            case 15 : //records
            case 19 : //rang 100%
            case 20 : //rang ultime
            case 21 : //rang de rapidit�
                gpGenerique->drawRecord(gpScreen2, gpKeyboard->getLigneRecord(),
                    gpKeyboard->getColonneRecord()); break;
            case 16 : //effacer record
                gpGenerique->drawEffacer(gpScreen2, gpKeyboard->getLigneVal()); break;
            default : break;
        }

        SDL_FreeSurface(gpScreen3);
        gpScreen3 = zoomSurface (gpScreen2, 2, 2, 0);
        SDL_BlitSurface(gpScreen3, &src, gpScreen, &dst);

        SDL_Flip(gpScreen);

        if (SDL_GetTicks() < lastAnimTime + 20) SDL_Delay(lastAnimTime+20-SDL_GetTicks());
        lastAnimTime = SDL_GetTicks();

    }

    SDL_FreeSurface(gpScreen2);
    SDL_FreeSurface(gpScreen3);

    delete gpEncyclopedie;
    delete gpGenerique;
    delete gpKeyboard;
    delete gpCarte;
    delete gpJeu;
    delete gpAudio;

    SDL_ShowCursor(SDL_ENABLE);
    SDL_Quit();
    exit(0);

    return 0;
}
