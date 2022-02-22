#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "os.h"

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn)
{
    uint64_t *table = phys_to_virt(pt << 12);
    int i, index;
    for (i = 0; i < 4; i++)
    {
        index = (vpn >> (36 - i * 9)) & 0x1FF;
        if (((table[index] & 1) == 0) && (ppn == NO_MAPPING))
        {
            return;
        }
        if ((table[index] & 1) == 0)
        {
            table[index] = (alloc_page_frame() << 12) + 1;
        }
        table = phys_to_virt(table[index] - 1);
    }
    index = vpn & 0x1FF;
    if (ppn == NO_MAPPING)
    {
        table[index] = (table[index]) & 0xFFFFFFFFFFFFFFFE;
    }
    else
    {
        table[index] = (ppn << 12) + 1;
    }
    return;
}

uint64_t page_table_query(uint64_t pt, uint64_t vpn)
{
    uint64_t *table = phys_to_virt(pt << 12);
    int i, index;
    for (i = 0; i < 4; i++)
    {
        index = (vpn >> (36 - i * 9)) & 0x1FF;
        if ((table[index] & 1) == 0)
        {
            return NO_MAPPING;
        }
        table = phys_to_virt(table[index] - 1);
    }
    index = vpn & 0x1FF;
    if ((table[index] & 1) == 0)
    {
        return NO_MAPPING;
    }
    return table[index] >> 12;
}