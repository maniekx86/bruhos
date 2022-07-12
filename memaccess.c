// https://stackoverflow.com/questions/37354717/displaying-text-video-memory-at-0xb8000-without-using-the-c-library

#define fastcall __attribute__((regparm(3)))
#define asmlinkage __attribute__((regparm(0)))

extern fastcall void dispchar(uint16_t celldata, uint16_t offset);

extern fastcall uint32_t getset_fs(uint32_t segment);
extern fastcall void set_fs(uint32_t segment);
extern fastcall uint32_t set_videomode_fs(void);
static inline uint16_t tm_charattr_to_celldata(uint8_t ochar, uint8_t attr);

#define VIDEO_SEG 0xb800

static inline uint16_t chr(uint8_t ochar, uint8_t attr)
{
    return (uint16_t) (attr << 8) | (uint8_t) ochar;
}


/* Display character with FS change */
fastcall void dispchar(uint16_t celldata, uint16_t offset)
{
    uint32_t oldfs = set_videomode_fs();
    dispchar_nofsupd(celldata, offset);
    set_fs(oldfs);
}

/* Display character with no FS change */
fastcall void dispchar_nofsupd(uint16_t celldata, uint16_t offset)
{
    __asm__ ("movw %w[wordval], %%fs:%[memloc]\n\t"
             :
             :[wordval]"ri"(celldata),
              [memloc] "m"(*(uint32_t *)(uint32_t)offset)
              :"memory");
}

/* Set FS segment and return previous value */
fastcall uint32_t getset_fs(uint32_t segment)
{
    uint32_t origfs;
    __asm__ __volatile__("mov %%fs, %w[origfs]\n\t"
                         "mov %w[segment], %%fs\n\t"
                         :[origfs] "=&rm"(origfs)
                         :[segment] "rm"(segment));
    return origfs;
}

/* Set FS segment */
fastcall void set_fs(uint32_t segment)
{
    __asm__("mov %w[segment], %%fs\n\t"
            :
            :[segment]"rm"(segment));
}

/* Set FS to video mode segment 0xb800 */
fastcall uint32_t set_videomode_fs(void)
{
    return getset_fs(VIDEO_SEG);
}


