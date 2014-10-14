#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TF1.h>
using namespace std;

//I (Phil) got this code from Ben Kay who got it from Jeff, 'a postdoc of considerable awesomeness', at Manchester. There are two changes from the original code which are noted at the lines where they occur.

//Compile using: g++ adsleyse.cpp `root-config --libs --cflags` -o adsleyse
//Possibly with more gubbins on Ubuntu if the library linking is being odd.

//If you need to talk to someone about this, I can try to help. I didn't write it so I might not be the right person, though. E-mail padsley@gmail.com. Also, honestly, the name is only that because I was trying to not get it confused with the original and this was an easy way to remember...

int main(int argc, char* argv[]){
	int i,j,nentries;
	double *v1,*v2,*v3;
	double *start,*mult,*q1,*q2,*q3,*q4,*q5,*q6,*q7,*q8;
	double *de1,*deo,*deu,*mpl,*mph;
	
	double anode1,anode,cath,cathmax,pm,pos,cog,cograw,qs[8];
	int fstart,fmult,fqs[8],from,to;
	float chi2,garea,gcent,gwidth;
	double sumxq=0,sumq=0;
	double scale,maxq=0;
	
	TH1D *temphist = new TH1D("temphist","pad charge",8,0,8);
	TF1 *gaussian = new TF1("gaussian","gaus");
	TF1 *gaus_sech = new TF1("gaus_sech","[0]*(0.75*exp(-0.5*((x-[1])/[2])**2) + 0.25/cosh(x*[1]))");
		
	string infile,outfile;
	
	infile = argv[1];
	outfile = infile;
	outfile.insert( outfile.find(".root"),"_an" );
	//cout << infile << " " << outfile << endl;

	TFile *inf = new TFile(infile.c_str());
	TFile *ouf = new TFile(outfile.c_str(),"RECREATE");
	
	TTree *tt = (TTree*)inf->Get("readout");
	TTree *nt = new TTree("readout","readout");
	
	nt->Branch("anode",&anode,"anode/D");
	nt->Branch("anode1",&anode1,"anode1/D");
	nt->Branch("cath",&cath,"cath/D");
	nt->Branch("pm",&pm,"pm/D");
	nt->Branch("pos",&pos,"pos/D");
	nt->Branch("cog",&cog,"cog/D");	
	nt->Branch("gwidth",&gwidth,"gwidth/F");	
	nt->Branch("chi2",&chi2,"chi2/F");	
	nt->Branch("mult",&fmult,"mult/I");
	
	nentries = tt->GetEntries();
	tt->SetEstimate(nentries);
	cout << "number of entries: " << nentries << endl;
	
	v1 = new double[nentries];
	v2 = new double[nentries];
	v3 = new double[nentries];
			
	start = new double[nentries];
	mult = new double[nentries];
	q1 = new double[nentries];
	q2 = new double[nentries];
	q3 = new double[nentries];
	q4 = new double[nentries];
	q5 = new double[nentries];
	q6 = new double[nentries];
	q7 = new double[nentries];
	q8 = new double[nentries];
	de1 = new double[nentries];
	deo = new double[nentries];
	deu = new double[nentries];
	mpl = new double[nentries];
	mph = new double[nentries];
	
	cout << "Reading..." << endl;
	tt->Draw("start:mult:q1","","goff");	
	v1 = tt->GetV1();
	v2 = tt->GetV2();
	v3 = tt->GetV3();

	for (i=0; i<nentries; i++){
		start[i] = v1[i];
		mult[i] = v2[i];
		q1[i] = v3[i];
	}

	cout << "Reading..." << endl;
	tt->Draw("q2:q3:q4","","goff");	
	v1 = tt->GetV1();
	v2 = tt->GetV2();
	v3 = tt->GetV3();

	for (i=0; i<nentries; i++){
		q2[i] = v1[i];
		q3[i] = v2[i];
		q4[i] = v3[i];
	}

	cout << "Reading..." << endl;
	tt->Draw("q5:q6:q7","","goff");	
	v1 = tt->GetV1();
	v2 = tt->GetV2();
	v3 = tt->GetV3();

	for (i=0; i<nentries; i++){
		q5[i] = v1[i];
		q6[i] = v2[i];
		q7[i] = v3[i];
	}

	cout << "Reading..." << endl;
	tt->Draw("q8:de1:deo","","goff");	
	v1 = tt->GetV1();
	v2 = tt->GetV2();
	v3 = tt->GetV3();

	for (i=0; i<nentries; i++){
		q8[i] = v1[i];
		de1[i] = v2[i];
		deo[i] = v3[i];
	}

	cout << "Reading..." << endl;
	tt->Draw("deu:mpl:mph","","goff");	
	v1 = tt->GetV1();
	v2 = tt->GetV2();
	v3 = tt->GetV3();

	for (i=0; i<nentries; i++){
		deu[i] = v1[i];
		mpl[i] = v2[i];
		mph[i] = v3[i];
	}
		
	for (i=0; i<nentries; i++){
// 		tt->Show(i);
// 		
// 		cout <<start[i]<<" "<<mult[i]<<" "<<q1[i]<<" "<<q2[i]<<" "<<q3[i]<<" "<<q4[i];
// 		cout <<" "<<q5[i]<<" "<<q6[i]<<" "<<q7[i]<<" "<<q8[i]<<" "<<de1[i]<<" "<<deo[i];
// 		cout <<" "<<deu[i]<<" "<<mpl[i]<<" "<<mph[i]<<endl;
		anode = deo[i]+deu[i];
		/*if(anode>32767 && anode<65535)
		  {
		    //cout << "anode= " << anode << endl;
		    anode -= 32768;
		  }
		else if(anode>65535)
		  {
		    //cout << "anode= " << anode << endl;
		    anode -= 65536;
		    }*/
		anode1 = de1[i];
		pm = mpl[i]+mph[i];

		qs[0]=q1[i]; qs[1]=q2[i]; qs[2]=q3[i]; qs[3]=q4[i];
		qs[4]=q5[i]; qs[5]=q6[i]; qs[6]=q7[i]; qs[7]=q8[i];

		fstart = (int)start[i];
		fmult = (int)mult[i];		
		for (j=0; j<8; j++) fqs[j]=0;
		
   	if(fstart%2){                   // odd start strip, discard first value
      	from = 1;
      	to = fmult;
      	fstart -= 1;
   	} else {                        // even start strip  
      	from = 0;
      	to = fmult-1;
	   }

		sumxq=0; sumq=0; maxq=0;
   	if (fmult > 0) {
   		for(j=from; j<=to; j++){
      		if (maxq < qs[j]) maxq = qs[j];
      		fqs[j] = qs[j];
      		sumxq += qs[j]*(double)j;
      		sumq  += qs[j];
   		}//end loop over active pads 
   	} //end check for hit strips

   	if(sumq <= 0) {
      	cog = 0; 
			pos = 0;
  	      scale = 0;
		}else if(fmult <= 2) {
      	cog = 0; 
			pos =0;
  	      scale = 0; 
		}else if(fstart <= 0) {
      	cog = 0; 
			pos = 0;
  	      scale = 0; 
		}else {
      	scale = 10;
      	cograw = sumxq / sumq;
      	cog = scale*((double)fstart + cograw);
      	cath = sumq;
			
   		for(j=1; j<=8; j++){
      		if(j <= fmult+1){
         		temphist->SetBinContent(j,fqs[j-1]);
         		if(fqs[j-1] < 1023) temphist->SetBinError(j,20);
         		else temphist->SetBinError(j,200);
      		} else {
         		temphist->SetBinContent(j,0);
         		temphist->SetBinError(j,0);
      		}
   		}//end loop over consecutive pads			
						
			garea = gcent = gwidth = pos = 0;			
    		if(fmult < 10){
      		gaussian->SetParameters(cath/3., cograw, 1.15);
      		gaussian->SetRange(0,(Float_t)fmult+1);
      		//temphist->Fit("gaussian", "QNR");
      		temphist->Fit("gaussian", "QNB"); //This has been changed from the line above the get the Gaussian fitting to work properly. Apparently.
				chi2 = gaussian->GetChisquare();
      		garea = gaussian->GetParameter(0);
				gcent = gaussian->GetParameter(1);
      		gwidth = gaussian->GetParameter(2);
      		pos = scale * ((Float_t)fstart + gcent -0.5);

		   } else {
      		gaus_sech->SetParameters(cath/3., cograw, 1.);
      		gaus_sech->SetRange(0,(Float_t)fmult);
		temphist->Fit("gaus_sech", "QRB");
		chi2 = gaus_sech->GetChisquare();
      		garea = gaus_sech->GetParameter(0);
      		pos = scale * (fstart + gaus_sech->GetParameter(1) -1);
    		} //end fit of gaussian or gaus_sech to pad charge distribution
			
		
    	}//end calculation of position by Center of Gravity and gaus fit
		cathmax = maxq;
		
		//cout << i << " " << cog << " " << gcent << " " << gwidth << endl;
		nt->Fill();
		if (i==0 || i%10000==0) {
			cout << "\rEvents processed: " << i << " " << (double)i/(double)nentries*100 << " %";
			cout.flush();
		}
		
	}//end loop over events
	ouf->cd();
	nt->Write();
	inf->Close();//The original code didn't close the input file which lead to errors which fecked it up. This line fixes that.
	ouf->Close();
        cout << "Dooooonnnnnnnnnnnnneeeeeeee" << endl;
	return 0;
}
