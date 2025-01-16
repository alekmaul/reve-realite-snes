.include "hdr.asm"

.section ".rodata1" superfree

tileset:
.incbin "gfxs/tiles.pic"
tilesetend:

tilepal:
.incbin "gfxs/tiles.pal"

mariogfx: .incbin "gfxs/mario_sprite.pic"
mariogfx_end:

mariopal: .incbin "gfxs/mario_sprite.pal"

snesfont: .incbin "gfxs/mariofont.pic"
snesfont_end:

snespal: .incbin "gfxs/mariofont.pal"

jumpsnd: .incbin "snds/mariojump.brr"
jumpsndend:

.ends

.section ".rodata2" superfree

mapmario:
.incbin "gfxs/BG1.m16"

objmario:
.incbin "gfxs/map_1_1.o16"

tilesetatt:
.incbin "gfxs/map_1_1.b16"

tilesetdef:
.incbin "gfxs/map_1_1.t16"

.ends
