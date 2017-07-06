#include <linux/init.h>
#include <linux/module.h>
#include <asm/string.h>

#define TOTAL_LETTERS 0
#define TOTAL_DIGITS 1
#define TOTAL_SYMBOLS 2

#define TOTAL_ALL TOTAL_LETTERS | TOTAL_DIGITS | TOTAL_SYMBOLS
#define STR_MAX_LEN 256

static int symbol_type = TOTAL_ALL;
static char *string = NULL;

unsigned int total_symbols(char* string, unsigned int total_type)
{
    printk("symbols: %s\n", string);
    return 0;
}

EXPORT_SYMBOL(total_symbols);

static int __init total_symbols_init(void)
{
    char type_str[STR_MAX_LEN];
    unsigned int number_of_symbols = 0;
    switch (symbol_type)
    {
        case TOTAL_LETTERS:
        {
            strcpy(type_str, "Letters");
        }
        break;

        case TOTAL_DIGITS:
        {
            strcpy(type_str, "Digits");
        }
        break;

        case TOTAL_SYMBOLS:
        {
            strcpy(type_str, "Symbols");
        }
        break;

        case TOTAL_ALL:
        default:
        {
            strcpy(type_str, "Characters");
        }
        break;
    }
    number_of_symbols = total_symbols(string, symbol_type);
    printk("<1>Total symbols module init!");
    printk("<1>%s: %d\n", type_str, number_of_symbols);
    return 0;
}

static void __exit total_symbols_exit(void)
{
    printk("<1>Total symbols module exit!");
}

/**
 ** 查看结果
 ** cat /var/log/messages | tail
 **/

module_init(total_symbols_init);
module_exit(total_symbols_exit);
module_param(symbol_type, uint, S_IRUGO);
module_param(string, charp, S_IRUGO);
MODULE_AUTHOR("Srefan");
MODULE_DESCRIPTION("A simple module program");
MODULE_VERSION("1.0.0");
