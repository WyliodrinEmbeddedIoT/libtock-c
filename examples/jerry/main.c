#include "jerry_library.h"


int main (void)
{
  jerry_setup();

  // jerry_execute ("var gpio = require ('gpio'); console.log ('pull: '+gpio.PULL_NONE);\
  //   console.log (gpio.enable_input(0,0)); console.log ('ioana'); console.log (gpio.enable_interrupt (0, 0));\
  //   console.log (gpio.interrupt_callback (function (pin, mode){\
  //     console.log ('pin: '+pin); console.log ('mode: '+mode);\
  //   }));");

  //jerry_execute ("var b = require ('button'); b.subscribe (function (a, b){console.log ('a '+a); console.log ('b '+b);}); b.enable_interrupt(0);");
  //jerry_execute ("var b = require ('button'); b.subscribe (function (a, c){console.log (a); console.log (c);}); b.subscribe (function (a, c){console.log ('handler');}); b.subscribe (function (a, c){if (c===1)console.log ('apasat');}); b.enable_interrupt(0);");
  //jerry_execute ("setInterval(function(){console.log ('interval');}, 2000); setTimeout(function (){console.log ('merge');}, 7000); setTimeout(function (){console.log ('merge a doua');}, 3000);");
  //jerry_execute ("setTimeout(function (){console.log ('merge');}, 7000); setTimeout(function (){console.log ('merge a doua');}, 3000);");
  //jerry_execute ("var b = require ('button'); var Button = b.Button; var btn = new Button (0, b.EDGE_BOTH); btn.watch(function (value){console.log (value);});");
  jerry_execute("var led = require ('led'); console.log ('count = '+led.count ()); var Led = led.Led; var l = new Led (0); l.on(); setTimeout (l.off, 2000);");
  while (1){yield();}

  //var led = require ('led'); console.log ('count = '+led.count ();) var Led = led.Led; var l = new Led (0); setTimeout (l.toggle, 1000);

  // jerry_execute ("var gpio = require ('gpio');\
  //   console.log (gpio.enable_input(0, 0)); while (1){console.log (gpio.read(0)); sleep();};");

  jerry_release();
  return 0;
}
