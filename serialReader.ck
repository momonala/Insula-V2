//serial test
SerialIO.list() @=> string list[]; // create array of available serial devices
for (int i; i<list.size(); i++){
    <<<i, ":", list[i] >>>; // print em out 
}
SerialIO cereal; //create instance of serial IO to communicate 
cereal.open(0, SerialIO.B115200, SerialIO.ASCII); //open port [port # from .list, baud, ASCII or BINARY]

SinOsc a => dac;
SinOsc b => dac;
SinOsc c => dac;
SinOsc d => dac;
//[a, b, c, d] @=> SinOsc osci[]; 
0.1 => float onGain; //volume 

while (true){
    cereal.onInts(4) => now; //reading of current line 
    cereal.getInts() @=>  int line[]; //cast reading to variable
    <<<line[0], line[1], line[2], line[3] >>>;    
    
    line[0]/3 => a.freq;
    onGain => a.gain;
    line[1]/3 => b.freq;
    onGain => b.gain;
    line[2]/3 => c.freq;
    onGain => c.gain;
    line[3]/3 => d.freq;
    onGain => d.gain;
    
    /*
    for (0 => int foo; foo < line.size(); foo++){
        line[foo]/3 => osci[foo].freq;
        onGain => osci[foo].gain;
    }*/ 
    10::ms => now;
}