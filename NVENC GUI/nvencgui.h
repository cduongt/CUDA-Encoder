#ifndef NVENCGUI_H
#define NVENCGUI_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtCore>
#include <QtGui>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QMessageBox>
#include <QFileDialog>

#include "ui_nvencgui.h"
#include "NvTranscoder.h"
#include <iostream>
#include <string>

using namespace std;

typedef struct
{
	unsigned long long lStart, lEnd, lFreq;
	double elapsedTime;
	int decodedFrames;
	int encodedFrames;
}TTranscodeResults;

typedef enum
{
	ERR_OK = 0,
	ERR_INPUT,
	ERR_OUTPUT,
	ERR_CUDA_INIT,
	ERR_CUDA_DEVICE,
	ERR_CUDA_CTX,
	ERR_CUDA_CTX_DESTROY,
	ERR_NVENC_ENC_INIT,
	ERR_NVENC_ENC_CREATE,
	ERR_NVENC_ENC_BUFFER
} errors;

class NVENCGUI : public QMainWindow
{
	Q_OBJECT

public:
	NVENCGUI(QWidget *parent = 0);
	~NVENCGUI();

private:
	Ui::NVENCGUIClass ui;
	EncodeConfig encodeConfig;
	TTranscodeResults results;
	int encodedFrames;
	void StartTranscoding();
	void Transcode();

private slots:
	void EncodeButtonClick();
	void PrintTranscodeResults();
	void PrintEncodingDetails();
	void PrintError(int);
	void SetInputFile();
	void SetOutputFileDirectory();
	void SetSameResolution();
	void SetSameFps();
	void IncrementEncodedFrames();

signals:
	void TranscodingEnd();
	void PrintDetails();
	void Error(int);
	void FrameEncoded();
};



#endif // NVENCGUI_H
