//
// Created by john on 02/09/2021.
//

#ifndef SNEKS_MEMORYCONTROLLER_H
#define SNEKS_MEMORYCONTROLLER_H

#include <cstddef>
#include <string>
#include <map>

namespace memory {

    class memoryController {

        //Todo: memory:
        //      Always mapped to the same locations : CPU registers, WorkRAM, PPU registers, OAM ColorGraphicsRAM, VideoRAM, Audio RAM, Joypad memory, interrupt vectors
        //      :
        //      Mapping modes: Rom image, StaticRAM, enhancement chips (I-RAM (co-processor memory), Control Registers (co-processor), BackupWorkRAM)
        //      :                  bank page byte
        //      VIRTUAL MEMORY SIZE: $FF FF FF
        //      :
        //      WRAM accessed at 2.68MHz            (($7E0000 - $7E1FFF) mirrored ($0000 to $1FFF)
        //      Romsel $000000-$7FFFFF 2.68MHz
        //      Romsel $800000-$FFFFFF 2.68 or 3.58MHz  (dependant on bit 0 of $420D (MEMSEL "register"), FastROM = 1, SlowROM = 0)
        //      :
        //      pages in remaining banks at $XX2000-$XX5FFF 3.58MHz
        //      bar pages $XX4000-$XX41FF limited to 1.78MHz
        //      :
        //      pages $XX6000 - $XX7FFF 2.68 MHz


    private:
        struct reg{
            int addr;
            int data;
            enum access{
                read=1,
                write=2,
                double_byte_write=4,
                word=8,
            } permission;

            reg() : addr(0), data(0), permission(access::read){}
            reg(int reg_addr, int reg_data)
                    : addr(reg_addr), data(reg_data), permission(access::read){}
            reg(int reg_addr, int reg_data, access perms)
                    : addr(reg_addr), data(reg_data), permission(perms){}
        };

        struct cart_data {
            int addr;
            char* data;
            int size;

            cart_data() : addr(0), data(nullptr), size(0){}
            cart_data(int reg_addr, char* data, int size)
                    : addr(reg_addr), data(data), size(size){}
        };

        //         24-bit
        int m_A_bus_len = 3;
        char8_t *m_A_bus;
        //         8-bit
        int m_B_bus_len = 1;

        char8_t *m_B_bus;
        //         8-bit
        int m_data_bus_len = 1;
        char8_t *m_data_bus;

        int m_map_mode;

        std::map<std::string, reg> m_registers;
        std::map<std::string, cart_data> m_cart_info;

        char8_t *m_ppu_registers;   //ppu registers
        char8_t *m_vram;    //video ram
        char16_t *m_cgram;   //color graphics ram
        char8_t *m_oam;     //object attribute memory

        char8_t* m_virtual_memory;  //the memory space that the cpu "sees"

        void init_registers();

        void init_A_bus();

        void init_B_bus();

        void init_data_bus();

        void setup_virtual_memory();

        void setup_ppu_memory();

        bool write_byte(char8_t *mem, int addr, char8_t data);

        bool write_bytes(char8_t *mem, int start_addr, char8_t *data, int len);

        bool write_nulls(char8_t *mem, int start_addr, int len);

        bool write_string(char8_t *mem, int start_addr, std::string data);

        void get_cart_info();

        void update_registers();

        char8_t* LoRom(int);

        void HiRom();

    public:

        memoryController();
        ~memoryController();

        bool load_rom_into_virtual_memory(char*, long int);

        char8_t *ABus() {
            return this->m_A_bus;
        }

        char8_t *BBus() {
            return this->m_B_bus;
        }

        char8_t *DataBus() {
            return this->m_data_bus;
        }

        std::map<std::string, cart_data> Cart_info() {
            return this->m_cart_info;
        }

        std::map<std::string, memoryController::reg> Registers() {
            return this->m_registers;
        }

        char8_t *VirtualMemory() {
            return this->m_virtual_memory;
        };

        char8_t *VRam(){
            return this->m_vram;
        }

        std::pair<std::string, reg> Register_from_address (int addr);

        char8_t *game_cart;

        char8_t get_byte(int addr);

        char8_t* get_bytes(int addr);

    };
}
#endif //SNEKS_MEMORYCONTROLLER_H
