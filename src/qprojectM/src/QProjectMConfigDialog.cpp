#include "QProjectMConfigDialog.hpp"
#include <QtDebug>


QProjectMConfigDialog::QProjectMConfigDialog(const std::string & configFile, QProjectMWidget * qprojectMWidget, QWidget * parent, Qt::WindowFlags f) : QDialog(parent, f), _configFile(configFile), _qprojectMWidget(qprojectMWidget) {
	
	qDebug() << "!!!";
	_ui.setupUi(this);
	connect(_ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonBoxHandler(QAbstractButton*)));
	connect(this, SIGNAL(projectM_Reset()), _qprojectMWidget, SLOT(resetProjectM()));
	populateMeshSizeComboBoxes();
	populateTextureSizeComboBox();

	loadConfig();
}

void QProjectMConfigDialog::buttonBoxHandler(QAbstractButton * button) {
	
	switch (_ui.buttonBox->standardButton(button)) {
		case QDialogButtonBox::Close:
			this->hide();
			break;
		case QDialogButtonBox::Apply:
			saveConfig();
			qDebug() << "emitting!";
			emit(projectM_Reset());
			break;
		default:
			break;
	}
}

void QProjectMConfigDialog::saveConfig() {
	qDebug() << "SAVE config";
	
	projectM::Settings settings = _qprojectMWidget->getQProjectM()->settings();
	
	settings.meshX = _ui.meshSizeXComboBox->itemData(_ui.meshSizeXComboBox->currentIndex()).toInt();
	settings.meshY = _ui.meshSizeYComboBox->itemData(_ui.meshSizeYComboBox->currentIndex()).toInt();
	settings.windowHeight = _ui.windowHeightSpinBox->value();
	settings.windowWidth = _ui.windowWidthSpinBox->value();		
	settings.titleFontURL = _ui.titleFontPathLineEdit->text().toStdString();
	settings.menuFontURL = _ui.menuFontPathLineEdit->text().toStdString();
	settings.presetURL = _ui.startupPlaylistLineEdit->text().toStdString();
	settings.textureSize = _ui.textureSizeComboBox->itemData(_ui.textureSizeComboBox->currentIndex()).toInt();
	settings.smoothPresetDuration = _ui.smoothPresetDurationSpinBox->value();
	settings.presetDuration = _ui.presetDurationSpinBox->value();
	settings.fps = _ui.maxFPSSpinBox->value();
	settings.aspectCorrection = _ui.useAspectCorrectionCheckBox->checkState() == Qt::Checked;
	settings.beatSensitivity = _ui.beatSensitivitySpinBox->value();
		
	projectM::writeConfig(_configFile, settings);
	
	qDebug() << "save end";
}


void QProjectMConfigDialog::populateMeshSizeComboBoxes() {

	
	for (int meshSize = 1<<1; meshSize < 1<<8; meshSize<<=1) {	
		_ui.meshSizeXComboBox->addItem(QString("%1").arg(meshSize), meshSize);
		_ui.meshSizeYComboBox->addItem(QString("%1").arg(meshSize), meshSize);		
	}
}


void QProjectMConfigDialog::populateTextureSizeComboBox() {

	
	for (int textureSize = 1<<1; textureSize < 1<<10; textureSize<<=1) {		
		_ui.textureSizeComboBox->addItem(QString("%1").arg(textureSize), textureSize);		
	}
}
void QProjectMConfigDialog::loadConfig() {
	
	qDebug() << "load config";
	const projectM::Settings & settings =(* _qprojectMWidget->getQProjectM()).settings();
	
	_ui.meshSizeXComboBox->insertItem(0, QString("%1 (current)").arg(settings.meshX), settings.meshX);
	_ui.meshSizeYComboBox->insertItem(0, QString("%1 (current)" ).arg(settings.meshY), settings.meshY);
	
	_ui.meshSizeXComboBox->setCurrentIndex(0);
	_ui.meshSizeYComboBox->setCurrentIndex(0);
	
	_ui.titleFontPathLineEdit->setText(settings.titleFontURL.c_str());
	_ui.menuFontPathLineEdit->setText(settings.menuFontURL.c_str());
	
	_ui.startupPlaylistLineEdit->setText(settings.presetURL.c_str());
	_ui.useAspectCorrectionCheckBox->setCheckState(settings.aspectCorrection ? Qt::Checked : Qt::Unchecked);
	_ui.maxFPSSpinBox->setValue(settings.fps);
	_ui.beatSensitivitySpinBox->setValue(settings.beatSensitivity);
	_ui.windowHeightSpinBox->setValue(settings.windowHeight);
	_ui.windowWidthSpinBox->setValue(settings.windowWidth);
	
	_ui.textureSizeComboBox->insertItem(0, QString("%1 (current)" ).arg(settings.textureSize), settings.textureSize);
	_ui.textureSizeComboBox->setCurrentIndex(0);
	
	_ui.smoothPresetDurationSpinBox->setValue(settings.smoothPresetDuration);
	_ui.presetDurationSpinBox->setValue(settings.presetDuration);
	
	qDebug() << "load config END";
		
}
