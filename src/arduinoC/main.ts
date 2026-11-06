//% color="#00b3a4" iconWidth=50 iconHeight=40
namespace lcd1602a {

    // Generator.addObject(id, type, code) emits:  <type> <code>;
    // so `code` must be the declarator only: "lcd(0x27)"
    function ensureLib() {
        Generator.addInclude('LCD1602A_h', '#include "LCD1602A.h"');
        Generator.addObject('lcd1602a_obj', 'LCD1602A', 'lcd(0x27)');
        Generator.addSetup('lcd1602a_init', 'lcd.begin();');
    }

    //% block="LCD1602 init | address [ADDR]" blockType="command"
    //% ADDR.shadow="dropdown" ADDR.options="ADDR"
    export function lcdInit(parameter: any, block: any) {
        var addr = parameter.ADDR.code;
        Generator.addInclude('LCD1602A_h', '#include "LCD1602A.h"');
        Generator.addObject('lcd1602a_obj', 'LCD1602A', `lcd(${addr})`);
        Generator.addSetup('lcd1602a_init', 'lcd.begin();');
    }

    //% block="LCD row [ROW] show [TEXT]" blockType="command"
    //% ROW.shadow="dropdown" ROW.options="ROW"
    //% TEXT.shadow="string" TEXT.defl="Hello micro:bit!"
    export function lcdPrint(parameter: any, block: any) {
        var row = parameter.ROW.code;
        var text = parameter.TEXT.code;
        var y = (row === '1') ? '0' : '1';
        ensureLib();
        Generator.addCode(`lcd.setCursor(0, ${y});lcd.print(${text});`);
    }

    //% block="LCD at col [X] row [ROW] show [TEXT]" blockType="command"
    //% X.shadow="number" X.defl=0
    //% ROW.shadow="dropdown" ROW.options="ROW"
    //% TEXT.shadow="string" TEXT.defl="AB"
    export function lcdPrintAt(parameter: any, block: any) {
        var x = parameter.X.code;
        var row = parameter.ROW.code;
        var text = parameter.TEXT.code;
        var y = (row === '1') ? '0' : '1';
        ensureLib();
        Generator.addCode(`lcd.setCursor(${x}, ${y});lcd.print(${text});`);
    }

    //% block="LCD clear screen" blockType="command"
    export function lcdClear(parameter: any, block: any) {
        ensureLib();
        Generator.addCode('lcd.clear();');
    }

    //% block="LCD backlight [STATE]" blockType="command"
    //% STATE.shadow="dropdown" STATE.options="STATE"
    export function lcdBacklight(parameter: any, block: any) {
        var state = parameter.STATE.code;
        ensureLib();
        Generator.addCode(`lcd.backlight(${state});`);
    }
}
