/**
 * Example for looping over protoDUNE channels and quality check
 * FIXME: 1) sst/FrameDataSource, configurable Claib_nChannel -> raw_nChannel, etc.
 * 2) sst/GeomDataSource does NOT fit protoDUNE structure, temporarily use a class ChannelGeo
 *
 */
#include "WireCellNav/SliceDataSource.h"
#include "WireCellSst/FrameDataSource.h"
//#include "WireCellSst/pDuneGeomDataSource.h"
#include "WireCellTiling/TileMaker.h"

#include "TFile.h"
#include "TTree.h"

#include <ctime>
#include <fstream>
#include <iostream>
using namespace std;

class ChannelGeo{
public:
  int chn;
  int tpc;
  int plane;
  int wire;
  double sx; // start of x
  double sy;
  double sz;
  double ex; // end of x
  double ey;
  double ez;
};

class ChannelMap{
  public:
    std::map<int, std::vector<ChannelGeo> > m_map;
    void add(ChannelGeo cg);
    std::vector<ChannelGeo> find(int chn);
    int channel_to_plane(int);
};

void ChannelMap::add(ChannelGeo cg){
    if(0==m_map.count(cg.chn)){
      std::vector<ChannelGeo> vv;
      vv.push_back(cg);
      m_map[cg.chn] =  vv;
    }
    else{
      // some-to-one mapping for wire-channel 
      m_map[cg.chn].push_back(cg);
    }
}

std::vector<ChannelGeo> ChannelMap::find(int chn){
  if(0==m_map.count(chn)) {
    std::cout << "Error. No channel found for " << chn << std::endl;
  }
  return m_map[chn];
}

int ChannelMap::channel_to_plane(int chn){
  std::vector<ChannelGeo> v_cg = find(chn);
  ChannelGeo cg = v_cg[0]; // only one channel-to-tpc mapping
  return cg.plane;
}


int main(int argc, char* argv[])
{

    if (argc < 2) {
	cerr << "usage: wire-cell-pdune-chan-qual sst-geometry.txt sst-celltree.root" << endl;
	exit (1);
    }

    // read the geometry file
    ifstream in(argv[1]);
    string dummyLine;
    getline(in, dummyLine); // ignore the first line
    ChannelMap chanmap;
    ChannelGeo cg;
    while(in >> cg.chn){
      in >> cg.tpc; in >> cg.plane; in >> cg.wire;
      in >> cg.sx; in >> cg.sy; in >> cg.sz; // in cm
      in >> cg.ex; in >> cg.ey; in >> cg.ez;
      chanmap.add(cg); 
    }
    in.close();



    time_t now=0, start_time = 0;

    // Get wire geometry
    /**
    * start_time = time(0);
    * ifstream geotext(argv[1]);
    * WireCellSst::pDuneGeomDataSource gds;
    * gds.load(geotext);
    * now = time(0);
    * cerr << "Loaded geometry in " << now - start_time << endl;
    */

    
    // One of the basic cell tilings
    /**
    * start_time = time(0);
    * WireCell::TileMaker tiling(gds);
    * now = time(0);
    * cerr << "Loaded tiling in " << now - start_time << endl;    
    */

    // open data file to make frame data source
    TFile* tfile = TFile::Open(argv[2]);
    TTree* tree = dynamic_cast<TTree*>(tfile->Get("/Event/Sim"));
    WireCellSst::FrameDataSource fds(*tree);
    
//    start_time = time(0);
//    WireCell::SliceDataSource sds(fds);
//    now = time(0);
//    cerr << "Loaded slice data source in " << now - start_time << endl;    
    
    // Loop over frames (aka "events")
    size_t nframes = fds.size();
    cout << "FDS: " << nframes << " frames" << endl;

//    start_time = time(0);
    nframes=1;
    for (size_t iframe = 0; iframe < nframes; ++iframe) {

	cout << "FDS: jumping to frame #" << iframe << endl;

	int iframe_got = fds.jump(iframe);
	if (iframe_got < 0) {
	    cerr << "Failed to get frame " << iframe << endl;
	    exit(1); // real code may want to do something less drastic
	}

        const WireCell::Frame& frame = fds.get();
        size_t ntraces = frame.traces.size();
        cout << "protoDUNE::frame.traces.size(): " << ntraces << endl;
        ntraces = 1;
        for (size_t ind=0; ind<ntraces; ++ind) {
          const WireCell::Trace& trace = frame.traces[ind];
          int tbin = trace.tbin;
          int chid = trace.chid;
          int nbins = trace.charge.size();
          //TH1F* hCharge = trace.hCharge;
          //cout << hCharge->GetMaximum() << endl;
          //cout << "chanId: " << chid <<  " tbin: " << tbin << " nbins: " << nbins << endl;
          //for(size_t itick=0; itick<nbins; ++itick){
          //  cout << "tick: " << itick+1 <<  " adc: " << trace.charge[itick] << endl;
          //}
          int iplane = chanmap.channel_to_plane(chid);
          cout << "chanId: " << chid << " plane: " << iplane << endl;
        }

	// loop over slices of the frame 

	//time_t start_frame_time = time(0);

	//size_t nslices = sds.size();
	//cout << "SDS: " << nslices << " slices in frame " << iframe_got << endl;
	//for (size_t islice = 0; islice < nslices; ++islice) {
	//    int islice_got = sds.jump(islice);
	//    if (islice_got < 0) {
	//	cerr << "Failed to get slice " << islice << " from frame " << iframe << endl;
	//	exit(1); // real code may want to do something less drastic
	//    }

	//    const WireCell::Slice& slice = sds.get();
	//    if (islice%1000 == 1) {
	//	time_t now = time(0);
	//	cerr << "slice #" << islice
	//	     << " time elapsed in frame: " << now-start_frame_time
	//	     << " slice tbin:" << slice.tbin() 
	//	     << " with " << slice.group().size() << " wires"
	//	     << endl;
	//    }

	//}

	//time_t now = time(0);
	//cerr << "frame #" << iframe
	//     << " time elapsed for frame: " << now-start_frame_time
	//     << " total time: " << now-start_time
	//     << endl;
    }

    return 0;

} // main()