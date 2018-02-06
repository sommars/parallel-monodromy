needs (currentFileDirectory|"../graph_creator.m2")

R = CC[a_1..a_5,b_1..b_5][x,y,z,t,u];
P = polySystem {-a_1+b_1*(x^2-y^2+z^2+u^2-t^2),-a_2+b_2*(x^3-y^3+z^3+u^3-t^3),-a_3+b_3*(x^4-y^4+z^4+u^4-t^4),-a_4+b_4*(x^5-y^5+z^5+u^5-t^5), -a_5+b_5*(x^6-y^6+z^6+u^6-t^6)};

NodeCount = 3
EdgeCount = 2

setRandomSeed 2
createRealGraph(P,NodeCount,EdgeCount,currentFileDirectory|"../../graphs/reimer5.graph");

getDefault CorrectorTolerance
setDefault(CorrectorTolerance=>1e-8) -- this induces some failures
setRandomSeed 2
createRealGraph(P,NodeCount,EdgeCount,currentFileDirectory|"../../graphs/reimer5.1e-8.graph");
