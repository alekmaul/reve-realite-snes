/*---------------------------------------------------------------------------------


    Simple game with map and sprite engine demo
    -- alekmaul


---------------------------------------------------------------------------------*/
#include <snes.h>

#include "soundbank.h"

//---------------------------------------------------------------------------------
extern char SOUNDBANK__;
extern char jumpsnd, jumpsndend;

//---------------------------------------------------------------------------------
#define GRAVITY 48
#define JUMPVALUE (GRAVITY * 20)

#define MARIO_MAXACCEL 0x0140
#define MARIO_ACCEL 0x0038
#define MARIO_JUMPING 0x0394
#define MARIO_HIJUMPING 0x0594

enum
{
    MARIODOWN = 0,
    MARIOJUMPING = 1,
    MARIOWALK = 2,
    MARIOSTAND = 6
}; // Mario state

//---------------------------------------------------------------------------------
extern char tileset, tilesetend, tilepal;
extern char tilesetdef, tilesetatt; // for map & tileset of map

extern char mapmario, objmario;

extern char mariogfx, mariogfx_end;
extern char mariopal;

extern char snesfont, snespal;

//---------------------------------------------------------------------------------
brrsamples Jump; // The sound for jumping

u16 pad0; // pad variable

t_objs *marioobj;             // pointer to mario object
s16 *marioox, *mariooy;       // basics x/y coordinates pointers with fixed point
s16 *marioxv, *marioyv;       // basics x/y velocity pointers with fixed point
u16 mariox, marioy;           // x & y coordinates of mario with map depth (not only screen)
u8 mariofidx, marioflp, flip; // to manage sprite display

//---------------------------------------------------------------------------------
// Init function for mario object
void marioinit(u16 xp, u16 yp, u16 type, u16 minx, u16 maxx)
{
    // to have a little message regarding init (DO NOT USE FOR REAL SNES GAME, JUST DEBUGGING PURPOSE)
    consoleNocashMessage("marioinit %d %d\n", (u16)xp, (u16)yp);

    // prepare new object
    if (objNew(type, xp, yp) == 0)
        // no more space, get out
        return;

    // Init some vars for snes sprite (objgetid is the current object id)
    objGetPointer(objgetid);
    marioobj = &objbuffers[objptr - 1];
    marioobj->width = 16;
    marioobj->height = 16;

    // grab the coordinates & velocity pointers
    marioox = (u16 *)&(marioobj->xpos + 1);
    mariooy = (u16 *)&(marioobj->ypos + 1);
    marioxv = (short *)&(marioobj->xvel);
    marioyv = (short *)&(marioobj->yvel);

    // update some variables for mario
    mariofidx = 0;
    marioflp = 0;
    marioobj->action = ACT_STAND;

    // prepare dynamic sprite object
    oambuffer[0].oamframeid = 6;
    oambuffer[0].oamrefresh = 1;
    oambuffer[0].oamattribute = 0x60 | (0 << 1); // palette 0 of sprite and sprite 16x16 and priority 2 and flip sprite
    oambuffer[0].oamgraphics = &mariogfx;

    // Init Sprites palette
    setPalette(&mariopal, 128 + 0 * 16, 16 * 2);
}

// Insert function for mario walk / jump / fall

//---------------------------------------------------------------------------------
// Update function for mario object
void marioupdate(u8 idx)
{
    // Insert code for moving mario

    // change sprite coordinates regarding map location
    mariox = (*marioox);
    marioy = (*mariooy);
    oambuffer[0].oamx = mariox - x_pos;
    oambuffer[0].oamy = marioy - y_pos;
    oamDynamic16Draw(0);

    // update camera regarding mario object
    mapUpdateCamera(mariox, marioy);
}

//---------------------------------------------------------------------------------
int main(void)
{
    // Initialize sound engine (take some time)
    spcBoot();

    // Initialize text console with our font
    consoleSetTextVramBGAdr(0x6000);
    consoleSetTextVramAdr(0x3000);
    consoleSetTextOffset(0x0000);
    consoleInitText(1, 16 * 2, &snesfont, &snespal);

    // Set give soundbank
    spcSetBank(&SOUNDBANK__);

    // allocate around 10K of sound ram (39 256-byte blocks)
    spcAllocateSoundRegion(39);

    // Load music
    spcLoad(MOD_OVERWORLD);

    // Load sample
    spcSetSoundEntry(15, 8, 6, &jumpsndend - &jumpsnd, &jumpsnd, &Jump);

    // Init layer with tiles and init also map length 0x6800 is mandatory for map engine
    bgSetGfxPtr(1, 0x3000);
    bgSetMapPtr(1, 0x6000, SC_32x32);
    bgInitTileSet(0, &tileset, &tilepal, 0, (&tilesetend - &tileset), 16 * 2, BG_16COLORS, 0x2000);
    bgSetMapPtr(0, 0x6800, SC_64x32);

    // Init sprite engine (0x0000 for large, 0x1000 for small)
    oamInitDynamicSprite(0x0000, 0x1000, 0, 0, OBJ_SIZE8_L16);

    // Object engine activate
    objInitEngine();

    // Init function for state machine
    objInitFunctions(0, &marioinit, &marioupdate, NULL);

    // Load all objects into memory
    objLoadObjects((char *)&objmario);

    // Load map in memory and update it regarding current location of the sprite
    mapLoad((u8 *)&mapmario, (u8 *)&tilesetdef, (u8 *)&tilesetatt);

    // Now Put in 16 color mode and disable BG3
    setMode(BG_MODE1, 0);
    bgSetEnable(1);
    bgSetDisable(2);

    // Put some text
    consoleDrawText(6, 17, "MARIOx00  WORLD TIME");
    consoleDrawText(6, 18, " 00000 ox00 1-1  000");

    // Put screen on
    setScreenOn();

    // Play file from the beginning
    spcPlay(0);
    spcSetModuleVolume(100);

    // Wait VBL 'and update sprites too ;-) )
    WaitForVBlank();

    // Wait for nothing :P
    while (1)
    {
        // Update the map regarding the camera
        mapUpdate();

        // Update all the available objects
        objUpdateAll();

        // prepare next frame and wait vblank
        oamInitDynamicSpriteEndFrame();
        spcProcess();
        WaitForVBlank();
        mapVblank();
        oamVramQueueUpdate();
    }
    return 0;
}
