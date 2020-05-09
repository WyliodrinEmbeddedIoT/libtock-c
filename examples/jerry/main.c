#include "jerry_library.h"


int main (void)
{
  jerry_setup();

  // jerry_execute ("var gpio = require ('gpio'); console.log ('pull: '+gpio.PULL_NONE);\
  //   console.log (gpio.enable_input(0,0)); console.log ('ioana'); console.log (gpio.enable_interrupt (0, 0));\
  //   console.log (gpio.interrupt_callback (function (pin, mode){\
  //     console.log ('pin: '+pin); console.log ('mode: '+mode);\
  //   }));");

  //jerry_execute ("var b = require ('button'); var Button = b.Button; var btn = new Button (0, 'rising'); btn.watch(function (value, err){console.log ('primul watch ' + value + ' ' + err);});");
  jerry_execute ("var Button = require ('onoff').Button; var b = new Button (0, 'rising'); b.watch(function (value, err){console.log ('primul watch ' + value);}); setTimeout(function(){b.unwatch(); console.log ('unwatch');},5000);");
  //jerry_execute("var led = require ('led'); console.log ('count = '+led.count ()); var Led = led.Led; var l = new Led (0); l.on(); setTimeout (l.off, 2000);");
  while (1){yield();}

  //var gpio= require ('gpio'); console.log ('count = '+gpio.count ();) var Gpio = gpio.Gpio; var l = new Gpio (0, 'out'); setTimeout (l.toggle, 1000);

  // jerry_execute ("var gpio = require ('gpio');\
  //   console.log (gpio.enable_input(0, 0)); while (1){console.log (gpio.read(0)); sleep();};");

  jerry_release();
  return 0;
}
