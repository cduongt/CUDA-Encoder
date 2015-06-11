#include "nvencgui.h"

/*
 * Main window constructor
 */
NVENCGUI::NVENCGUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.progressBar->setValue(0);
	ui.comboBoxPreset->addItem("High quality");
	ui.comboBoxPreset->addItem("High performance");
	//ui.comboBoxPreset->addItem("Low latency high quality");
	//ui.comboBoxPreset->addItem("Low latency high performance");
	encodedFrames = 0;
	qRegisterMetaType<QTextCursor>("QTextCursor");
	//qRegisterMetaType<QTextBlock>("QTextBlock");
	QObject::connect(this, SIGNAL(TranscodingEnd()), this, SLOT(PrintTranscodeResults()));
	QObject::connect(this, SIGNAL(PrintDetails()), this, SLOT(PrintEncodingDetails()));
	QObject::connect(this, SIGNAL(Error(int)), this, SLOT(PrintError(int)));
	QObject::connect(ui.checkBoxSameResolution, SIGNAL(stateChanged(int)), this, SLOT(SetSameResolution()));
	QObject::connect(ui.checkBoxSameFps, SIGNAL(stateChanged(int)), this, SLOT(SetSameFps()));
	QObject::connect(this, SIGNAL(FrameEncoded()), this, SLOT(IncrementEncodedFrames()));
}

/*
 * Destructor
 */
NVENCGUI::~NVENCGUI()
{

}

/*
 * Blocks/Unblocks height/width spinboxes depending on state of checkbox
 */

void NVENCGUI::SetSameResolution()
{
	if (ui.checkBoxSameResolution->isChecked())
	{
		ui.spinBoxWidth->setEnabled(false);
		ui.spinBoxHeight->setEnabled(false);
	}
	else
	{
		ui.spinBoxWidth->setEnabled(true);
		ui.spinBoxHeight->setEnabled(true);
	}
}

/*
 * Blocks/Unblocks fps spinbox depending on state of checkbox
 */

void NVENCGUI::SetSameFps()
{
	if (ui.checkBoxSameFps->isChecked()) ui.spinBoxFps->setEnabled(false);
	else ui.spinBoxFps->setEnabled(true);
}

/*
 * Encoded Frames progress update
 */

void NVENCGUI::IncrementEncodedFrames()
{
	encodedFrames++;
	
	if (encodedFrames % 100 == 0)
	{
		ui.plainTextEdit->appendPlainText(("Encoded Frames: " + to_string(encodedFrames)).c_str());
		ui.plainTextEdit->moveCursor(QTextCursor::End);
		ui.plainTextEdit->repaint();
	}
	/*
	ui.plainTextEdit->appendPlainText("test");
	ui.plainTextEdit->setFocus();
	ui.plainTextEdit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
	ui.plainTextEdit->moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
	ui.plainTextEdit->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
	ui.plainTextEdit->textCursor().removeSelectedText();
	ui.plainTextEdit->textCursor().deletePreviousChar();
	ui.plainTextEdit->appendPlainText(("Encoded Frames: " + to_string(encodedFrames)).c_str());*/
	
}

/*
 * Action after pushing "Encode!" button
 * Disables button, which is activated after transcoding ends
 * Also clears text box
 */
void NVENCGUI::EncodeButtonClick()
{
	ui.pushButton->setEnabled(false);
	ui.plainTextEdit->clear();
	StartTranscoding();
}

/*
 * Prints encoding details before transcoding starts
 */
void NVENCGUI::PrintEncodingDetails()
{
	ui.plainTextEdit->appendPlainText("Encoding started!\n");
	ui.plainTextEdit->appendPlainText(("Input             : " + string(encodeConfig.inputFileName)).c_str());
	ui.plainTextEdit->appendPlainText(("Output            : " + string(encodeConfig.outputFileName)).c_str());
	if (encodeConfig.codec == NV_ENC_HEVC) 	ui.plainTextEdit->appendPlainText("Codec             : HEVC");
	else ui.plainTextEdit->appendPlainText("Codec             : H264");
	ui.plainTextEdit->appendPlainText(("Size              : " + to_string(encodeConfig.width) + "x" + to_string(encodeConfig.height)).c_str());
	if (encodeConfig.bitrate > 0) ui.plainTextEdit->appendPlainText(("Bitrate           : " + to_string(encodeConfig.bitrate / 1000) + " kbps").c_str());
	if (encodeConfig.vbvMaxBitrate > 0) ui.plainTextEdit->appendPlainText(("vbvMaxBitrate     : " + to_string(encodeConfig.vbvMaxBitrate)).c_str());
	if (encodeConfig.vbvSize > 0) ui.plainTextEdit->appendPlainText(("vbvSize             : " + to_string(encodeConfig.vbvSize)).c_str());
	ui.plainTextEdit->appendPlainText(("Fps               : " + to_string(encodeConfig.fps)).c_str());
	string rc;
	rc = encodeConfig.rcMode == NV_ENC_PARAMS_RC_CONSTQP ? "Constant QP Mode" : 
		encodeConfig.rcMode == NV_ENC_PARAMS_RC_VBR ? "Variable bitrate mode" :
		encodeConfig.rcMode == NV_ENC_PARAMS_RC_CBR ? "Constant bitrate mode" :
		encodeConfig.rcMode == NV_ENC_PARAMS_RC_VBR_MINQP ? "Variable bitrate mode with MinQP" :
		encodeConfig.rcMode == NV_ENC_PARAMS_RC_2_PASS_QUALITY ? "Multi pass encoding" :
		encodeConfig.rcMode == NV_ENC_PARAMS_RC_2_PASS_FRAMESIZE_CAP ? "Multi pass encoding for frame size" :
		encodeConfig.rcMode == NV_ENC_PARAMS_RC_2_PASS_VBR ? "Multi pass variable bitrate mode" : "UNKNOWN";
	ui.plainTextEdit->appendPlainText(("Rate control mode : " + rc).c_str());
	if (encodeConfig.gopLength == NVENC_INFINITE_GOPLENGTH) ui.plainTextEdit->appendPlainText("GOP Length        : Infinite GOP length");
	else ui.plainTextEdit->appendPlainText(("GOP Length        : " + to_string(encodeConfig.gopLength)).c_str());
	if (encodeConfig.numB > 0) ui.plainTextEdit->appendPlainText(("B Frames          : " + to_string(encodeConfig.numB)).c_str());
	if (encodeConfig.qp > 0) ui.plainTextEdit->appendPlainText(("QP                : " + to_string(encodeConfig.qp)).c_str());
	string preset;
	preset = encodeConfig.presetGUID == NV_ENC_PRESET_LOW_LATENCY_HQ_GUID ? "Low latency high quality" :
		encodeConfig.presetGUID == NV_ENC_PRESET_LOW_LATENCY_HP_GUID ? "Low latency high performance" :
		encodeConfig.presetGUID == NV_ENC_PRESET_HQ_GUID ? "High quality" :
		encodeConfig.presetGUID == NV_ENC_PRESET_HP_GUID ? "High performance" :
		encodeConfig.presetGUID == NV_ENC_PRESET_LOSSLESS_HP_GUID ? "High performance lossless" : "Low latency default";
	ui.plainTextEdit->appendPlainText(("Preset            : " + preset).c_str());
	ui.plainTextEdit->appendPlainText("\n");
	ui.plainTextEdit->repaint();
}

/*
 * Sets encoding parameters from input and starts the transcoding thread
 */
void NVENCGUI::StartTranscoding()
{
	encodeConfig = { 0 };
	encodeConfig.endFrameIdx = INT_MAX;
	//encodeConfig.bitrate = 5000000;
	encodeConfig.bitrate = ui.spinBoxBitrate->value() * 1000;
	encodeConfig.rcMode = NV_ENC_PARAMS_RC_CBR;
	encodeConfig.gopLength = NVENC_INFINITE_GOPLENGTH;
	encodeConfig.codec = NV_ENC_H264;
	if (!ui.checkBoxSameFps->isChecked()) encodeConfig.fps = ui.spinBoxFps->value();
	if (encodeConfig.rcMode == NV_ENC_PARAMS_RC_CONSTQP) encodeConfig.qp = 28;
	if (!ui.checkBoxSameResolution->isChecked())
	{
		encodeConfig.width = ui.spinBoxWidth->value();
		encodeConfig.height = ui.spinBoxHeight->value();
	}
	if (ui.comboBoxPreset->currentIndex() == 0) encodeConfig.encoderPreset = "hq";
	else if (ui.comboBoxPreset->currentIndex() == 1) encodeConfig.encoderPreset = "hp";
	else if (ui.comboBoxPreset->currentIndex() == 2) encodeConfig.encoderPreset = "lowLatencyHQ";
	else if (ui.comboBoxPreset->currentIndex() == 3) encodeConfig.encoderPreset = "lowLatencyHP";

	encodeConfig.presetGUID = NV_ENC_PRESET_DEFAULT_GUID;
	encodeConfig.pictureStruct = NV_ENC_PIC_STRUCT_FRAME;
	//encodeConfig.inputFileName = "testh264.mp4";
	encodeConfig.inputFileName = strdup(ui.lineEditInput->text().toStdString().c_str());
	//encodeConfig.outputFileName = "output\\output.h264";
	if (ui.lineEditOutput->text() == "" || ui.lineEditOutputFile->text() == "")
	{
		emit Error(ERR_OUTPUT);
		return;
	}
	encodeConfig.outputFileName = strdup((ui.lineEditOutput->text() + "/" + ui.lineEditOutputFile->text()).toStdString().c_str());

	QFuture<void> future = QtConcurrent::run(this, &NVENCGUI::Transcode);
}

/*
 * Prints error passed by int code
 */
void NVENCGUI::PrintError(int error)
{
	QMessageBox messageBox;
	string message;
	switch (error)
	{
	case ERR_INPUT:
		if (encodeConfig.inputFileName) message = "Can't open input file: " + string(encodeConfig.inputFileName);
		else message = "Input file not specified";
		break;
	case ERR_OUTPUT:
		if (encodeConfig.outputFileName) message = "Can't open output file: " + string(encodeConfig.outputFileName);
		else message = "Output file not specified";
		break;
	case ERR_CUDA_INIT:
		message = "Unable to initialize CUDA";
		break;
	case ERR_CUDA_DEVICE:
		message = "Unable to initialize CUDA device";
		break;
	case ERR_CUDA_CTX:
		message = "Unable to create CUDA context";
		break;
	case ERR_CUDA_CTX_DESTROY:
		message = "Unable to destroy CUDA context";
		break;
	case ERR_NVENC_ENC_INIT:
		message = "Unable to initialize NVENC Encoder";
		break;
	case ERR_NVENC_ENC_CREATE:
		message = "Unable to create NVENC Encoder for specified file";
		break;
	case ERR_NVENC_ENC_BUFFER:
		message = "Unable to get resources for Encoder IO Buffer";
		break;
	}
	messageBox.setText(message.c_str());
	messageBox.exec();
	ui.pushButton->setEnabled(true);
}

/*
 * Prints result time, encoded/decoded frames etc. after transcoding ends
 */
void NVENCGUI::PrintTranscodeResults()
{
	ui.plainTextEdit->appendPlainText(("\nTotal time: " + to_string(results.elapsedTime) + " seconds").c_str());
	ui.plainTextEdit->appendPlainText(("Decoded Frames : " + to_string(results.decodedFrames)).c_str());
	ui.plainTextEdit->appendPlainText(("Encoded Frames : " + to_string(results.encodedFrames)).c_str());
	ui.plainTextEdit->appendPlainText(("Average FPS : " + to_string(results.encodedFrames / results.elapsedTime)).c_str());
	ui.pushButton->setEnabled(true);
}

/*
 * Sets input file by file dialog
 */
void NVENCGUI::SetInputFile()
{
	ui.lineEditInput->setText(QFileDialog::getOpenFileName(this, tr("Select input file"), QDir::currentPath()));
}

/*
 * Sets output directory
 */
void NVENCGUI::SetOutputFileDirectory()
{
	ui.lineEditOutput->setText(QFileDialog::getExistingDirectory(this, tr("Select output directory"), QDir::currentPath(), QFileDialog::ShowDirsOnly));
}

/*
 * Main transcoding thread
 * Initializes CUDA device, decodes frames with NVCUVID API and adds them to frame queue, which passes them to NVENC for encoding, then output
 */
void NVENCGUI::Transcode()
{
	CUresult result;

	// initialize CUDA
	result = cuInit(0);
	if (result != CUDA_SUCCESS)
	{
		emit Error(ERR_CUDA_INIT);
		return;
	}

	NVENCSTATUS nvStatus = NV_ENC_SUCCESS;

	// no input file
	if (encodeConfig.inputFileName == NULL)
	{
		emit Error(ERR_INPUT);
		return;
	}

	// no output file
	if (encodeConfig.outputFileName == NULL)
	{
		emit Error(ERR_OUTPUT);
		return;
	}

	// unable to open input file
	if (!fopen(encodeConfig.inputFileName, "r"))
	{
		emit Error(ERR_INPUT);
		return;
	}

	encodeConfig.fOutput = fopen(encodeConfig.outputFileName, "wb");
	// unable to open output file
	if (encodeConfig.fOutput == NULL)
	{
		emit Error(ERR_OUTPUT);
		return;
	}

	// initialize CUDA on device and set CUDA context
	CUcontext cudaCtx;
	CUdevice device;

	result = cuDeviceGet(&device, encodeConfig.deviceID);
	if (result != CUDA_SUCCESS)
	{
		emit Error(ERR_CUDA_DEVICE);
		return;
	}
	result = cuCtxCreate(&cudaCtx, CU_CTX_SCHED_AUTO, device);
	if (result != CUDA_SUCCESS)
	{
		emit Error(ERR_CUDA_CTX);
		return;
	}

	// initialize NVCUVID context
	CUcontext curCtx;
	CUvideoctxlock ctxLock;
	result = cuCtxPopCurrent(&curCtx);
	if (result != CUDA_SUCCESS)
	{
		emit Error(ERR_CUDA_CTX);
		return;
	}
	result = cuvidCtxLockCreate(&ctxLock, curCtx);
	if (result != CUDA_SUCCESS)
	{
		emit Error(ERR_CUDA_CTX);
		return;
	}

	CudaDecoder* pDecoder = new CudaDecoder;
	FrameQueue* pFrameQueue = new CUVIDFrameQueue(ctxLock);
	pDecoder->InitVideoDecoder(encodeConfig.inputFileName, ctxLock, pFrameQueue, encodeConfig.width, encodeConfig.height);

	int decodedW, decodedH, decodedFRN, decodedFRD;
	pDecoder->GetCodecParam(&decodedW, &decodedH, &decodedFRN, &decodedFRD);

	// If the width/height is not set, set to same as source
	if (encodeConfig.width <= 0 || encodeConfig.height <= 0) {
		encodeConfig.width = decodedW;
		encodeConfig.height = decodedH;
	}

	// same, except for fps
	if (encodeConfig.fps <= 0) {
		if (decodedFRN <= 0 || decodedFRD <= 0)
			encodeConfig.fps = 30;
		else
			encodeConfig.fps = decodedFRN / decodedFRD;
	}

	// initialize frame queue with width/height
	pFrameQueue->init(encodeConfig.width, encodeConfig.height);

	VideoEncoder* pEncoder = new VideoEncoder(ctxLock);
	assert(pEncoder->GetHWEncoder());

	// initialize NVENC HW Encoder
	nvStatus = pEncoder->GetHWEncoder()->Initialize(cudaCtx, NV_ENC_DEVICE_TYPE_CUDA);
	if (nvStatus != NV_ENC_SUCCESS)
	{
		emit Error(ERR_NVENC_ENC_INIT);
		return;
	}

	// get preset GUID
	encodeConfig.presetGUID = pEncoder->GetHWEncoder()->GetPresetGUID(encodeConfig.encoderPreset, encodeConfig.codec);

	// create encoder
	nvStatus = pEncoder->GetHWEncoder()->CreateEncoder(&encodeConfig);
	if (nvStatus != NV_ENC_SUCCESS)
	{
		emit Error(ERR_NVENC_ENC_CREATE);
		return;
	}

	// create buffer
	nvStatus = pEncoder->AllocateIOBuffers(&encodeConfig);
	if (nvStatus != NV_ENC_SUCCESS)
	{
		emit Error(ERR_NVENC_ENC_BUFFER);
		return;
	}

	// print details to text window, start counter
	emit PrintDetails();
	NvQueryPerformanceCounter(&results.lStart);

	//start decoding thread
#ifdef _WIN32
	HANDLE decodeThread = CreateThread(NULL, 0, DecodeProc, (LPVOID)pDecoder, 0, NULL);
#else
	pthread_t pid;
	pthread_create(&pid, NULL, DecodeProc, (void*)pDecoder);
#endif

	int encodedFrames = 0;

	//start encoding thread
	while (!(pFrameQueue->isEndOfDecode() && pFrameQueue->isEmpty())) 
	{
		CUVIDPARSERDISPINFO pInfo;
		if (pFrameQueue->dequeue(&pInfo)) 
		{
			CUdeviceptr dMappedFrame = 0;
			unsigned int pitch;
			CUVIDPROCPARAMS oVPP = { 0 };
			oVPP.unpaired_field = 1;
			oVPP.progressive_frame = 1;

			cuvidMapVideoFrame(pDecoder->GetDecoder(), pInfo.picture_index, &dMappedFrame, &pitch, &oVPP);

			EncodeFrameConfig stEncodeConfig = { 0 };
			stEncodeConfig.dptr = dMappedFrame;
			stEncodeConfig.pitch = pitch;
			stEncodeConfig.width = encodeConfig.width;
			stEncodeConfig.height = encodeConfig.height;
			pEncoder->EncodeFrame(&stEncodeConfig);

			cuvidUnmapVideoFrame(pDecoder->GetDecoder(), dMappedFrame);
			pFrameQueue->releaseFrame(&pInfo);
			//emit IncrementEncodedFrames();
		}
	}

	// flush
	pEncoder->EncodeFrame(NULL, true);

	// end decoding thread
#ifdef _WIN32
	WaitForSingleObject(decodeThread, INFINITE);
#else
	pthread_join(pid, NULL);
#endif

	// print transcoding details
	if (pEncoder->GetEncodedFrames() > 0)
	{
		results.decodedFrames = pDecoder->m_decodedFrames;
		results.encodedFrames = pEncoder->GetEncodedFrames();

		NvQueryPerformanceCounter(&results.lEnd);
		NvQueryPerformanceFrequency(&results.lFreq);
		results.elapsedTime = (double)(results.lEnd - results.lStart) / (double)results.lFreq;
	}
	emit TranscodingEnd();

	// clean up

	cuvidCtxLockDestroy(ctxLock);
	pEncoder->Deinitialize();
	delete pDecoder;
	delete pEncoder;
	delete pFrameQueue;

	result = cuCtxDestroy(cudaCtx);
	if (result != CUDA_SUCCESS)
	{
		emit Error(ERR_CUDA_CTX_DESTROY);
		return;
	}

	return;
}