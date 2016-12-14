<<<"\n", "Welcome to Insula. Prepare for a fantasic musical jounrey.", "\n">>>;

//--------Open Serial Connectopn ----------------------------------------------------
SerialIO.list() @=> string list[]; // create array of available serial devices
for (int i; i<list.size(); i++){
    <<<i, ":", list[i] >>>; // print em out 
}

SerialIO cereal; // instance of serial IO to communicate 
cereal.open(0, SerialIO.B115200, SerialIO.ASCII); //open port [port# from list, baud, ASCII or BINARY]
//make sure serial is reading at right positions in array
//read a few lines, check to see if "100" marker is at right position
//blackhole if not
for (0 => int i; i < 5; i++){
    cereal.onInts(15) => now; //reading of current line 
    cereal.getInts() @=>  int line[]; //cast reading to variable 
    //<<<line[0], line[1], line[2], line[3], line[4], line[5], line[6], line[7], line[8], line[9], line[10], line[11], line[12], line[13], line[14] >>>;    
    if (i == 4 && line[0] != 100){
        <<<"poor data stream... replace shred">>>;       
        SinOsc s => blackhole; 
        1::week => now; 
    }
}

//--------Intialize Oscillators ----------------------------------------------------
//STKinstruments: Clarinet, BlowBotl, Flute, Saxofony, Brass, ModalBar, BandedWG. Moog
0.05 => float fGain; //volume 
SinOsc f_osc[4]; //oscillators for fingers 
for (0 => int i; i < f_osc.cap(); i++){ 
    f_osc[i] => dac; //connect each to DAC 
}

0.1 => float aGain; //volume 
Saxofony a_osc[3]; // oscillators for accel
for (0 => int i; i < a_osc.cap(); i++){ 
    a_osc[i] => dac; //connect each to DAC
} 

//===============Infinite Loop =======================================================
while (true){
    cereal.onInts(15) => now; //reading of current line 
    cereal.getInts() @=>  int line[]; //cast reading to variable
    
    <<<line[0], line[1], line[2], line[3], line[4], line[5], line[6], line[7], line[8], line[9], line[10], line[11], line[12], line[13], line[14] >>>;    
    line[0] => int check;
    line[1] => int f1; //for readability 
    line[2] => int v1;
    line[3] => int f2;
    line[4] => int v2;
    line[5] => int f3;
    line[6] => int v3;
    line[7] => int f4;
    line[8] => int v4;
    line[9] => int a1;
    line[10] => int a2;
    line[11] => int a3;
    line[12] => int av1;
    line[13] => int av2;
    line[14] => int av3;
   
    //[f1, f2, f3, f4] @=> int f[]; //flex values array  
    //[v1, v2, v3, f4] @=> int v[]; //finger velocities array 
    //[a1,a2,a3] @=> int a[]; //acceleration array
    //[av1,av2,av3] @=> int av[]; //velocity of acceleration array 

    fingerScream(f1, f2, f3, f4);
    accelSwoosh(a1, a2, a3, av1, av2, av3);
    
    10::ms => now;
}

//=======================FUNCTIONS ==================================================
//--------Fingers Fuction ----------------------------------------------------
//PICK DESIRED EFFECT 
"click" => string type;
//"smooth" => string type;

fun void fingerScream(int f1, int f2, int f3, int f4){
    [f1, f2, f3, f4] @=> int f[]; //flex values array 
    for (0 => int i; i < f_osc.cap(); i++){
        if (f[i] > 25){
            if (type == "click"){               //clicky effect 
                Std.mtof(aTune(f[i])) => f_osc[i].freq; //autotune, convert midi to freq
            }
            else if (type == "smooth"){          // smooth effect
                 f[i]*2 => f_osc[i].freq;
            }
            //1 => f_osc[i].noteOn;   //COMMENT FOR SINOSC   
            fGain => f_osc[i].gain; //switch on all oscillators  
        }
        else{
            0 => f_osc[i].gain; 
            //1 => f_osc[i].noteOff; //COMMENT FOR SINOSC
        } 
    }
}  

//-----------Accel Function ----------------------------------------------------
fun void accelSwoosh(int a1, int a2, int a3, int av1, int av2, int av3){
    [a1,a2,a3] @=> int a[]; //acceleration array 
    [av1,av2,av3] @=> int av[]; //velo of accel array 
    for (0 => int i; i < a.cap(); i++){
        //a_osc[i].Delay(10);
        if (Std.abs(av[i]) > 0){ //if we sense a change in position 
            Std.mtof(aTune(Std.abs(a[i]))) => a_osc[i].freq;
            aGain => a_osc[i].gain;
            1 => a_osc[i].noteOn;
        }
        else{
            1 => a_osc[i].noteOff;
            0 => a_osc[i].gain;
        }
    }
}

//--------Autotune (C for now) ----------------------------------------------------
[1,3,6,8,10] @=> int offNotes[];//autotune array of out-of-key midi notes, can change the key sig
fun int aTune(int rawNote){
    int aTuneNote;
    rawNote % 12 => int checkNote;
    //check if note (within 8 note scale) is in offNote array 
    if (checkNote == offNotes[0] || checkNote == offNotes[1] || checkNote == offNotes[2] || checkNote == offNotes[3] || checkNote == offNotes[4]){
        rawNote + 1 => aTuneNote; //autotune if off key 
    }
    else {
        rawNote => aTuneNote; //keep if in key 
    }
    return aTuneNote; 
}
