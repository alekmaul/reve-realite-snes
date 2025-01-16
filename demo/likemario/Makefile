export PVSNESLIB_HOME=../pvsneslib

ifeq ($(strip $(PVSNESLIB_HOME)),)
$(error "Please create an environment variable PVSNESLIB_HOME by following this guide: https://github.com/alekmaul/pvsneslib/wiki/Installation")
endif

# BEFORE including snes_rules :
# list in AUDIOFILES all your .it files in the right order. It will build to generate soundbank file
AUDIOFILES := snds/overworld.it
# then define the path to generate soundbank data. The name can be different but do not forget to update your include in .c file !
export SOUNDBANK := soundbank

include ${PVSNESLIB_HOME}/devkitsnes/snes_rules

.PHONY: bitmaps all

#---------------------------------------------------------------------------------
# ROMNAME is used in snes_rules file
export ROMNAME := likemario

# to build musics, define SMCONVFLAGS with parameters you want
SMCONVFLAGS	:= -s -o $(SOUNDBANK) -V -b 5
musics: $(SOUNDBANK).obj

all: musics snds/mariojump.brr bitmaps $(ROMNAME).sfc

clean: cleanBuildRes cleanRom cleanGfx cleanAudio
	@rm -f *.clm gfxs/*.b16 gfxs/*.o16  gfxs/*.t16 gfxs/*.pal gfxs/*.pic gfxs/*.map snds/mariojump.brr

#---------------------------------------------------------------------------------
gfxs/mario_sprite.pic: gfxs/mario_sprite.bmp
	@echo convert sprites ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t bmp -i $<

gfxs/tiles.pic: gfxs/tiles.png
	@echo convert map tileset... $(notdir $@)
	$(GFXCONV) -s 8 -o 16 -u 16 -p -m -i $<

gfxs/map_1_1.m16: gfxs/map_1_1.tmj gfxs/tiles.pic
	@echo convert map tiled ... $(notdir $@)
	$(TMXCONV) $< gfxs/tiles.map

gfxs/mariofont.pic: gfxs/mariofont.bmp
	@echo convert font with no tile reduction ... $(notdir $@)
	$(GFXCONV) -s 8 -o 2 -u 16 -e 1 -p -t bmp -m -R -i $<

bitmaps : gfxs/tiles.pic gfxs/mariofont.pic gfxs/map_1_1.m16 gfxs/mario_sprite.pic
