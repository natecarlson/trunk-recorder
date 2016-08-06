#ifndef P25_RECORDER_H
#define P25_RECORDER_H

#define _USE_MATH_DEFINES

#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#include <boost/shared_ptr.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <gnuradio/io_signature.h>
#include <gnuradio/hier_block2.h>
#include <gnuradio/blocks/multiply_const_ff.h>
#include <gnuradio/filter/firdes.h>
#include <gnuradio/filter/fir_filter_ccf.h>
#include <gnuradio/filter/fir_filter_fff.h>
#include <gnuradio/filter/freq_xlating_fir_filter_ccf.h>
#include <gnuradio/filter/firdes.h>
#include <gnuradio/filter/rational_resampler_base_ccc.h>

#include <gnuradio/analog/quadrature_demod_cf.h>

#include <gnuradio/analog/sig_source_c.h>
#include <gnuradio/analog/feedforward_agc_cc.h>

#include <gnuradio/digital/diff_phasor_cc.h>

#include <gnuradio/blocks/complex_to_arg.h>


#include <gnuradio/blocks/multiply_cc.h>
#include <gnuradio/blocks/multiply_const_ff.h>
#include <gnuradio/blocks/multiply_const_cc.h>

#include <gnuradio/blocks/file_sink.h>
#include <gnuradio/filter/pfb_arb_resampler_ccf.h>
#include <gnuradio/filter/rational_resampler_base_ccf.h>
#include <gnuradio/filter/rational_resampler_base_fff.h>

#include <gnuradio/block.h>
#include <gnuradio/blocks/null_sink.h>

#include <gnuradio/blocks/copy.h>

#include <gnuradio/blocks/short_to_float.h>
#include <gnuradio/blocks/char_to_float.h>

#include <op25_repeater/fsk4_demod_ff.h>
#include <op25_repeater/fsk4_slicer_fb.h>
#include <op25_repeater/p25_frame_assembler.h>
#include <op25_repeater/gardner_costas_cc.h>
#include <op25_repeater/vocoder.h>
#include <gnuradio/msg_queue.h>
#include <gnuradio/message.h>
#include <gnuradio/blocks/head.h>
#include <gnuradio/blocks/file_sink.h>

#include "recorder.h"
#include "../../gr_blocks/nonstop_wavfile_sink.h"
#include "../../gr_blocks/freq_xlating_fft_filter.h"
#include "../../gr_blocks/latency_probe.h"
#include "../../gr_blocks/latency_tagger.h"

class Source;
class p25_recorder;
typedef boost::shared_ptr<p25_recorder> p25_recorder_sptr;
p25_recorder_sptr make_p25_recorder(Source *src, bool qpsk);
#include "../source.h"

class p25_recorder : public gr::hier_block2, public Recorder
{
	friend p25_recorder_sptr make_p25_recorder(Source *src, bool qpsk);
protected:
	p25_recorder(Source *src, bool qpsk);

public:
	~p25_recorder();

	void tune_offset(double f);
	void activate( Call *call, int n);
	std::vector<unsigned long> get_active_probe_offsets();
	std::vector<double> get_active_probe_delays();
	std::vector<unsigned long> get_last_probe_offsets();
	std::vector<double> get_last_probe_delays();
	void clear_probes();
	void deactivate();
	void close();
	double get_freq();
	int get_num();
	double get_current_length();
	bool is_active();
	State get_state();
	int lastupdate();
	long elapsed();
	long closing_elapsed();
    Source *get_source();
	gr::msg_queue::sptr tune_queue;
	gr::msg_queue::sptr traffic_queue;
	gr::msg_queue::sptr rx_queue;
	//void forecast(int noutput_items, gr_vector_int &ninput_items_required);

private:
	double center, freq;
	bool muted;
  bool qpsk_mod;
	long talkgroup;
	time_t timestamp;
	time_t starttime;
	time_t closing_time;

  Source *source;
	char filename[160];
	char raw_filename[160];
	//int num;

	bool iam_logging;
	State state;


	//std::vector<gr_complex> lpf_coeffs;
		std::vector<float> lpf_coeffs;
	std::vector<float> arb_taps;
	std::vector<float> sym_taps;
freq_xlating_fft_filter_sptr prefilter;
latency_tagger_sptr tagger;
latency_probe_sptr active_probe;
latency_probe_sptr last_probe;
	/* GR blocks */
	gr::filter::fir_filter_ccf::sptr lpf;
	gr::filter::fir_filter_fff::sptr sym_filter;

	gr::analog::sig_source_c::sptr lo;

gr::digital::diff_phasor_cc::sptr diffdec;

	gr::blocks::multiply_cc::sptr mixer;
	gr::blocks::file_sink::sptr fs;

	gr::filter::pfb_arb_resampler_ccf::sptr arb_resampler;
	//gr::filter::freq_xlating_fir_filter_ccf::sptr prefilter;
	gr::filter::rational_resampler_base_ccf::sptr downsample_sig;
	gr::filter::rational_resampler_base_fff::sptr upsample_audio;

	gr::analog::quadrature_demod_cf::sptr fm_demod;
	gr::analog::feedforward_agc_cc::sptr agc;

	gr::blocks::nonstop_wavfile_sink::sptr wav_sink;

	gr::blocks::short_to_float::sptr converter;
	gr::blocks::copy::sptr valve;

	gr::blocks::multiply_const_ff::sptr multiplier;
	gr::blocks::multiply_const_ff::sptr rescale;
	gr::blocks::multiply_const_ff::sptr baseband_amp;
	gr::blocks::complex_to_arg::sptr to_float;
	gr::op25_repeater::fsk4_demod_ff::sptr fsk4_demod;
	gr::op25_repeater::p25_frame_assembler::sptr op25_frame_assembler;

	gr::op25_repeater::fsk4_slicer_fb::sptr slicer;
	gr::op25_repeater::vocoder::sptr op25_vocoder;
	gr::op25_repeater::gardner_costas_cc::sptr costas_clock;
};


#endif
