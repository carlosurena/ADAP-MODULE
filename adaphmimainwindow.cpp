#include "adaphmimainwindow.h"
#include "ui_adaphmimainwindow.h"
#include "adaphmiwidgetstate.h"
#include <QSignalMapper>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QMessageBox>
#include <QFileDialog>
#include "adaphmixmlcreator.h"

AdapHmiMainWindow::AdapHmiMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdapHmiMainWindow)
{
    ui->setupUi(this);
    AdapHmiMainWindow::createSlotsForAdapHmiStackedWidget();
    AdapHmiMainWindow::createSlotsForInputOutputParameterStackedWidget();
    AdapHmiMainWindow::initializeAllWidgets();
    AdapHmiMainWindow::createObjectConnects();
}
AdapHmiMainWindow::~AdapHmiMainWindow()
{
    delete ui;
}
void AdapHmiMainWindow::createSlotsForAdapHmiStackedWidget(){
    ui->adapHMIStackedWidget->setCurrentIndex(0);
    ui->selectAdapStratLookupTableButton->setFocus();
    QSignalMapper* adapSignalMapper = new QSignalMapper(this);
    QObject::connect(ui->selectAdapStratLookupTableButton,SIGNAL(clicked()),adapSignalMapper, SLOT(map()));
    QObject::connect(ui->reviewBlockDiagramBackButton,SIGNAL(clicked()),adapSignalMapper, SLOT(map()));
    QObject::connect(ui->reviewBlockDiagramApplyButton,SIGNAL(clicked()),adapSignalMapper, SLOT(map()));
    QObject::connect(ui->adapParametersBackButton,SIGNAL(clicked()),adapSignalMapper, SLOT(map()));
    QObject::connect(ui->adapParametersApplyButton,SIGNAL(clicked()),adapSignalMapper, SLOT(map()));
    adapSignalMapper->setMapping(ui->selectAdapStratLookupTableButton, 1);
    adapSignalMapper->setMapping(ui->reviewBlockDiagramBackButton, 0);
    adapSignalMapper->setMapping(ui->reviewBlockDiagramApplyButton, 2);
    adapSignalMapper->setMapping(ui->adapParametersBackButton, 1);
    adapSignalMapper->setMapping(ui->adapParametersApplyButton, 3);
    QObject::connect(adapSignalMapper, SIGNAL(mapped(int)), this, SLOT(sendToNextAdapHmiStackedWidget(int)));
}
void AdapHmiMainWindow::createSlotsForInputOutputParameterStackedWidget(){
    ui->inputOutputParameterStackedWidget->setCurrentIndex(0);
    QSignalMapper* adapSignalMapper = new QSignalMapper(this);
    QObject::connect(ui->arcWeldingInputParametersFrontButton,SIGNAL(clicked()),adapSignalMapper, SLOT(map()));
    QObject::connect(ui->arcWeldingOutputParametersBackButton,SIGNAL(clicked()),adapSignalMapper, SLOT(map()));
    QObject::connect(ui->arcWeldingOutputParametersFrontButton,SIGNAL(clicked()),adapSignalMapper, SLOT(map()));
    QObject::connect(ui->arcWeldingLookUpTableBackButton,SIGNAL(clicked()),adapSignalMapper, SLOT(map()));
    adapSignalMapper->setMapping(ui->arcWeldingInputParametersFrontButton, 1);
    adapSignalMapper->setMapping(ui->arcWeldingOutputParametersBackButton, 0);
    adapSignalMapper->setMapping(ui->arcWeldingOutputParametersFrontButton, 2);
    adapSignalMapper->setMapping(ui->arcWeldingLookUpTableBackButton, 1);
    QObject::connect(adapSignalMapper, SIGNAL(mapped(int)), this, SLOT(sendToNextInputOutputParameterStackedWidget(int)));
}
void AdapHmiMainWindow::sendToNextInputOutputParameterStackedWidget(int index){
    AdapHmiMainWindow::retractAllWidgets();
    if(index == 1){
        if(!AdapHmiMainWindow::inputParametersValidation()){
            index = 0;
        }
    }
    if(index == 2){
        if(!AdapHmiMainWindow::outputParametersValidation()){
            index = 1;
        } else {
            ui->adapParametersApplyButton->setEnabled(true);
        }
    }
    if(index == 2 && arcWeldingLookUpTableState->getWidgetState()){
        arcWeldingLookUpTableState->setWidgetState(ui->arcWeldingLookUpTableView,false);
        AdapHmiMainWindow::createlutTableBasedOnInputOutputParameters();
    }
    ui->inputOutputParameterStackedWidget->setCurrentIndex(index);
}
void AdapHmiMainWindow::sendToNextAdapHmiStackedWidget(int index){
    AdapHmiMainWindow::retractAllWidgets();
    if(index == 2){
        if(!AdapHmiMainWindow::adapArcWeldingParametersValidation()){
            index = 1;
        }
    }
    if(index == 3){
        AdapHmiMainWindow::createBlockDiagramFromQMap();
    }
    ui->adapHMIStackedWidget->setCurrentIndex(index);
}
void AdapHmiMainWindow::showReviewBlockDiagramWeldingParametersWidgetOnClicked(){
    AdapHmiMainWindow::retractGivenWidget(540,495,301,80,reviewBlockDiagramSettingsWidgetState,ui->reviewBlockDiagramSettingsWidget);
    AdapHmiMainWindow::retractExpandGivenWidget(176,495,118,80,reviewBlockDiagramWeldingParametersWidgetState,ui->reviewBlockDiagramWeldingParametersWidget);
}
void AdapHmiMainWindow::showArcWeldingParametersWidgetOnClick(){
    ui->arcWeldingWireFeedLineEdit->setFocus();
    AdapHmiMainWindow::retractExpandGivenWidget(690,30,311,455,arcWeldingParametersWidgetState,ui->arcWeldingParametersWidget);
}
void AdapHmiMainWindow::showArcWeldingAdditionalParameterWidgetOnClick(){
    ui->arcWeldingWeavingReqYesRadioButton->setFocus();
    AdapHmiMainWindow::retractGivenWidget(540,495,301,80,reviewBlockDiagramSettingsWidgetState,ui->reviewBlockDiagramSettingsWidget);
    AdapHmiMainWindow::retractGivenWidget(176,495,118,80,reviewBlockDiagramWeldingParametersWidgetState,ui->reviewBlockDiagramWeldingParametersWidget);
    AdapHmiMainWindow::retractExpandGivenWidget(10,310,291,131,arcWeldingAdditionalParameterWidgetState,ui->arcWeldingAdditionalParameterWidget);
}
void AdapHmiMainWindow::showReviewBlockDiagramSettingsWidgetOnClick(){
    AdapHmiMainWindow::retractGivenWidget(176,495,118,80,reviewBlockDiagramWeldingParametersWidgetState,ui->reviewBlockDiagramWeldingParametersWidget);
    AdapHmiMainWindow::retractExpandGivenWidget(540,495,301,80,reviewBlockDiagramSettingsWidgetState,ui->reviewBlockDiagramSettingsWidget);
}
void AdapHmiMainWindow::showAdapParametersSettingsWidgetOnClick(){
    AdapHmiMainWindow::retractExpandGivenWidget(540,495,301,80,adapParametersSettingsWidgetState,ui->adapParametersSettingsWidget);
}
void AdapHmiMainWindow::showArcWeldingInputParametersVisionInputWidgetOnClick(){
    AdapHmiMainWindow::retractGivenWidget(540,495,301,80,adapParametersSettingsWidgetState,ui->adapParametersSettingsWidget);
    AdapHmiMainWindow::retractGivenWidget(110,90,91,301,arcWeldingInputParametersFilteringLevelWidgetState,ui->arcWeldingInputParametersFilteringLevelWidget);
    AdapHmiMainWindow::retractGivenWidget(210,90,91,301,arcWeldingInputParametersNoMatchBehaviourWidgetState,ui->arcWeldingInputParametersNoMatchBehaviourWidget);
    AdapHmiMainWindow::retractExpandGivenWidget(10,90,91,301,arcWeldingInputParametersVisionInputWidgetState,ui->arcWeldingInputParametersVisionInputWidget);
}
void AdapHmiMainWindow::showArcWeldingInputParametersFilteringLevelWidgetOnClick(){
    AdapHmiMainWindow::retractGivenWidget(540,495,301,80,adapParametersSettingsWidgetState,ui->adapParametersSettingsWidget);
    AdapHmiMainWindow::retractGivenWidget(10,90,91,301,arcWeldingInputParametersVisionInputWidgetState,ui->arcWeldingInputParametersVisionInputWidget);
    AdapHmiMainWindow::retractGivenWidget(210,90,91,301,arcWeldingInputParametersNoMatchBehaviourWidgetState,ui->arcWeldingInputParametersNoMatchBehaviourWidget);
    AdapHmiMainWindow::retractExpandGivenWidget(110,90,91,301,arcWeldingInputParametersFilteringLevelWidgetState,ui->arcWeldingInputParametersFilteringLevelWidget);
}
void AdapHmiMainWindow::showArcWeldingInputParametersNoMatchBehaviourWidgetOnClick(){
    AdapHmiMainWindow::retractGivenWidget(540,495,301,80,adapParametersSettingsWidgetState,ui->adapParametersSettingsWidget);
    AdapHmiMainWindow::retractGivenWidget(10,90,91,301,arcWeldingInputParametersVisionInputWidgetState,ui->arcWeldingInputParametersVisionInputWidget);
    AdapHmiMainWindow::retractGivenWidget(110,90,91,301,arcWeldingInputParametersFilteringLevelWidgetState,ui->arcWeldingInputParametersFilteringLevelWidget);
    AdapHmiMainWindow::retractExpandGivenWidget(210,90,91,301,arcWeldingInputParametersNoMatchBehaviourWidgetState,ui->arcWeldingInputParametersNoMatchBehaviourWidget);
}
void AdapHmiMainWindow::showArcWeldingOutputParametersBodyWidgetOnClick(){
    AdapHmiMainWindow::retractGivenWidget(540,495,301,80,adapParametersSettingsWidgetState,ui->adapParametersSettingsWidget);
    AdapHmiMainWindow::retractExpandGivenWidget(10,96,291,301,arcWeldingOutputParametersBodyWidgetState,ui->arcWeldingOutputParametersBodyWidget);
}
void AdapHmiMainWindow::showArcWeldingOutputAdditionalParameterWidgetOnSelection(bool checked){
    AdapHmiMainWindow::retractGivenWidget(540,495,301,80,adapParametersSettingsWidgetState,ui->adapParametersSettingsWidget);
    if(checked){
        AdapHmiMainWindow::retractExpandGivenWidget(10,214,271,81,arcWeldingOutputAdditionalParameterWidgetState,ui->arcWeldingOutputAdditionalParameterWidget);
        lutInputOutputMap.insert("INPUT_WEAVINGREQ","YES");
        ui->arcWeldingAmplitudeLineEdit->setEnabled(true);
        ui->arcWeldingAmplitudeLineEdit->setStyleSheet("background-color: rgb(255, 255, 255)");
        ui->arcWeldingAmplitudeLineEdit->setFocus();
    }
}
void AdapHmiMainWindow::performActionArcWeldingWeavingReqNoRadioButtonOnClick(bool checked){
    AdapHmiMainWindow::retractGivenWidget(540,495,301,80,adapParametersSettingsWidgetState,ui->adapParametersSettingsWidget);
    if(checked){
        if(!arcWeldingOutputAdditionalParameterWidgetState->getWidgetState()){
            AdapHmiMainWindow::retractExpandGivenWidget(10,214,271,81,arcWeldingOutputAdditionalParameterWidgetState,ui->arcWeldingOutputAdditionalParameterWidget);
        }
        lutInputOutputMap.insert("INPUT_WEAVINGREQ","NO");
        ui->arcWeldingAmplitudeLineEdit->clear();
        ui->arcWeldingFrequencyLineEdit->clear();
        ui->arcWeldingAmplitudeLineEdit->setEnabled(false);
        ui->arcWeldingFrequencyLineEdit->setEnabled(false);
        ui->arcWeldingAmplitudeLineEdit->setStyleSheet("background-color: rgb(203, 203, 203)");
        ui->arcWeldingFrequencyLineEdit->setStyleSheet("background-color: rgb(203, 203, 203)");
        if(lutInputOutputMap.contains("INPUT_AMPLITUDE")){
                lutInputOutputMap.remove("INPUT_AMPLITUDE");
        }
        if(lutInputOutputMap.contains("INPUT_FREQUENCY")){
                lutInputOutputMap.remove("INPUT_FREQUENCY");
        }
        ui->reviewBlockDiagramApplyButton->setFocus();
    }
}

void AdapHmiMainWindow::showArcWeldingLookUpTableViewOnClick(){
    AdapHmiMainWindow::retractGivenWidget(540,495,301,80,adapParametersSettingsWidgetState,ui->adapParametersSettingsWidget);
    AdapHmiMainWindow::retractExpandGivenWidget(10,100,291,291,arcWeldingLookUpTableViewState,ui->arcWeldingLookUpTableView);
}
void AdapHmiMainWindow::retractExpandWidgets(int X, int Y, int W, int H, RequestTypes type, QObject *qInputWidget){
    QParallelAnimationGroup toggleAnimation;
    QPropertyAnimation *animation = new QPropertyAnimation(qInputWidget, "geometry");
    animation->setDuration(5000);
    QRect startRect;
    QRect endRect;
    switch(type){
    case Retract:
         startRect.setRect(X,Y,W,H);
         endRect.setRect(X,Y,0,0);
         animation->setStartValue(startRect);
         animation->setEndValue(endRect);
         toggleAnimation.addAnimation(animation);
         toggleAnimation.start();
         break;
    case Expand:
         startRect.setRect(X,Y,0,0);
         endRect.setRect(X,Y,W,H);
         animation->setStartValue(startRect);
         animation->setEndValue(endRect);
         toggleAnimation.addAnimation(animation);
         toggleAnimation.start();
         break;
    default:
         break;
    }
}
void AdapHmiMainWindow::retractAllWidgets(){
    AdapHmiMainWindow::retractGivenWidget(540,495,301,80,reviewBlockDiagramSettingsWidgetState,ui->reviewBlockDiagramSettingsWidget);
    AdapHmiMainWindow::retractGivenWidget(176,495,118,80,reviewBlockDiagramWeldingParametersWidgetState,ui->reviewBlockDiagramWeldingParametersWidget);
    AdapHmiMainWindow::retractGivenWidget(690,30,311,455,arcWeldingParametersWidgetState,ui->arcWeldingParametersWidget);
    AdapHmiMainWindow::retractGivenWidget(10,310,291,131,arcWeldingAdditionalParameterWidgetState,ui->arcWeldingAdditionalParameterWidget);
    AdapHmiMainWindow::retractGivenWidget(10,90,91,301,arcWeldingInputParametersVisionInputWidgetState,ui->arcWeldingInputParametersVisionInputWidget);
    AdapHmiMainWindow::retractGivenWidget(110,90,91,301,arcWeldingInputParametersFilteringLevelWidgetState,ui->arcWeldingInputParametersFilteringLevelWidget);
    AdapHmiMainWindow::retractGivenWidget(210,90,91,301,arcWeldingInputParametersNoMatchBehaviourWidgetState,ui->arcWeldingInputParametersNoMatchBehaviourWidget);
    AdapHmiMainWindow::retractGivenWidget(10,96,291,301,arcWeldingOutputParametersBodyWidgetState,ui->arcWeldingOutputParametersBodyWidget);
    AdapHmiMainWindow::retractGivenWidget(540,495,301,80,adapParametersSettingsWidgetState,ui->adapParametersSettingsWidget);
    AdapHmiMainWindow::retractGivenWidget(10,100,291,291,arcWeldingLookUpTableViewState,ui->arcWeldingLookUpTableView);
}
void AdapHmiMainWindow::retractGivenWidget(int X, int Y, int W, int H,adaphmiwidgetstate *givenWidgetState,QWidget *givenWidget){
    if(!givenWidgetState->getWidgetState()){
        givenWidgetState->setWidgetState(givenWidget,true);
        AdapHmiMainWindow::retractExpandWidgets(X,Y,W,H,Expand,givenWidget);
    }
}
void AdapHmiMainWindow::retractExpandGivenWidget(int X, int Y, int W, int H,adaphmiwidgetstate *givenWidgetState,QWidget *givenWidget){
    if (givenWidgetState->getWidgetState()) {
        givenWidgetState->setWidgetState(givenWidget,false);
        AdapHmiMainWindow::retractExpandWidgets(X,Y,W,H,Retract,givenWidget);
    } else {
        givenWidgetState->setWidgetState(givenWidget,true);
        AdapHmiMainWindow::retractExpandWidgets(X,Y,W,H,Expand,givenWidget);
    }
}
void AdapHmiMainWindow::createObjectConnects(){
    QObject::connect(ui->reviewBlockDiagramWeldingParamToolButton, SIGNAL(clicked()),this, SLOT(showReviewBlockDiagramWeldingParametersWidgetOnClicked()));
    QObject::connect(ui->reviewBlockDiagramArcWeldingToolButton, SIGNAL(clicked()),this, SLOT(showArcWeldingParametersWidgetOnClick()));
    QObject::connect(ui->arcWeldingAdditionalParameterPushButton, SIGNAL(clicked()),this, SLOT(showArcWeldingAdditionalParameterWidgetOnClick()));
    QObject::connect(ui->reviewBlockDiagramSettingsToolButton, SIGNAL(clicked()),this, SLOT(showReviewBlockDiagramSettingsWidgetOnClick()));
    QObject::connect(ui->adapParametersSettingsToolButton, SIGNAL(clicked()),this, SLOT(showAdapParametersSettingsWidgetOnClick()));
    QObject::connect(ui->arcWeldingInputParametersOneVisionInputToolButton, SIGNAL(clicked()),this, SLOT(showArcWeldingInputParametersVisionInputWidgetOnClick()));
    QObject::connect(ui->arcWeldingInputParametersFilteringLevelToolButton, SIGNAL(clicked()),this, SLOT(showArcWeldingInputParametersFilteringLevelWidgetOnClick()));
    QObject::connect(ui->arcWeldingInputParametersNoMatchToolButton, SIGNAL(clicked()),this, SLOT(showArcWeldingInputParametersNoMatchBehaviourWidgetOnClick()));
    QObject::connect(ui->arcWeldingOutputParametersToolButton, SIGNAL(clicked()),this, SLOT(showArcWeldingOutputParametersBodyWidgetOnClick()));
    QObject::connect(ui->arcWeldingWeavingReqYesRadioButton, SIGNAL(clicked(bool)),this, SLOT(showArcWeldingOutputAdditionalParameterWidgetOnSelection(bool)));
    QObject::connect(ui->arcWeldingWeavingReqNoRadioButton, SIGNAL(clicked(bool)),this, SLOT(performActionArcWeldingWeavingReqNoRadioButtonOnClick(bool)));
    QObject::connect(ui->arcWeldingLookUpTableToolButton, SIGNAL(clicked()),this, SLOT(showArcWeldingLookUpTableViewOnClick()));
    QObject::connect(ui->arcWeldingWireFeedLineEdit, SIGNAL(editingFinished()), this, SLOT(storeArcWeldingWireFeedLineEdit()));
    QObject::connect(ui->arcWeldingCurrentLineEdit, SIGNAL(editingFinished()), this, SLOT(storeArcWeldingCurrentLineEdit()));
    QObject::connect(ui->arcWeldingVoltageLineEdit, SIGNAL(editingFinished()), this, SLOT(storeArcWeldingVoltageLineEdit()));
    QObject::connect(ui->arcWeldingTravelSpeedLineEdit, SIGNAL(editingFinished()), this, SLOT(storeArcWeldingTravelSpeedLineEdit()));
    QObject::connect(ui->arcWeldingAmplitudeLineEdit, SIGNAL(editingFinished()), this, SLOT(storeArcWeldingAmplitudeLineEdit()));
    QObject::connect(ui->arcWeldingFrequencyLineEdit, SIGNAL(editingFinished()), this, SLOT(storeArcWeldingFrequencyLineEdit()));
    QObject::connect(ui->arcWeldingInputParametersAreaCheckBox, SIGNAL(clicked(bool)),this, SLOT(storeArcWeldingInputParametersAreaCheckBoxOnSelection(bool)));
    QObject::connect(ui->arcWeldingInputParametersGapCheckBox, SIGNAL(clicked(bool)),this, SLOT(storeArcWeldingInputParametersGapCheckBoxOnSelection(bool)));
    QObject::connect(ui->arcWeldingInputParametersMismatchCheckBox, SIGNAL(clicked(bool)),this, SLOT(storeArcWeldingInputParametersMismatchCheckBoxOnSelection(bool)));
    QObject::connect(ui->arcWeldingInputParametersMaintainOutputBehaviourRadioButton, SIGNAL(clicked(bool)),this, SLOT(storeArcWeldingInputParametersMaintainOutputBehaviourRadioButtonOnSelection(bool)));
    QObject::connect(ui->arcWeldingInputParametersResetToNormalBehaviourRadioButton, SIGNAL(clicked(bool)),this, SLOT(storeArcWeldingInputParametersResetToNormalBehaviourRadioButtonOnSelection(bool)));
    QObject::connect(ui->arcWeldingInputParametersHighFilteringLevelRadioButton, SIGNAL(clicked(bool)),this, SLOT(storeArcWeldingInputParametersHighFilteringLevelRadioButtonOnSelection(bool)));
    QObject::connect(ui->arcWeldingInputParametersMediumFilteringLevelRadioButton, SIGNAL(clicked(bool)),this, SLOT(storeArcWeldingInputParametersMediumFilteringLevelRadioButtonOnSelection(bool)));
    QObject::connect(ui->arcWeldingInputParametersNoneFilteringLevelRadioButton, SIGNAL(clicked(bool)),this, SLOT(storeArcWeldingInputParametersNoneFilteringLevelRadioButtonOnSelection(bool)));
    QObject::connect(ui->arcWeldingInputParametersSmallFilteringLevelRadioButton, SIGNAL(clicked(bool)),this, SLOT(storeArcWeldingInputParametersSmallFilteringLevelRadioButtonOnSelection(bool)));
    QObject::connect(ui->arcWeldingOutputAmplitudeCheckBox, SIGNAL(clicked(bool)),this, SLOT(storeArcWeldingOutputAmplitudeCheckBoxOnSelection(bool)));
    QObject::connect(ui->arcWeldingOutputFrequencyCheckBox, SIGNAL(clicked(bool)),this, SLOT(storeArcWeldingOutputFrequencyCheckBoxOnSelection(bool)));
    QObject::connect(ui->arcWeldingOutputCurrentCheckBox, SIGNAL(clicked(bool)),this, SLOT(storeArcWeldingOutputCurrentCheckBoxOnSelection(bool)));
    QObject::connect(ui->arcWeldingOutputTravelSpeedCheckBox, SIGNAL(clicked(bool)),this, SLOT(storeArcWeldingOutputTravelSpeedCheckBoxOnSelection(bool)));
    QObject::connect(ui->arcWeldingOutputVoltageCheckBox, SIGNAL(clicked(bool)),this, SLOT(storeArcWeldingOutputVoltageCheckBoxOnSelection(bool)));
    QObject::connect(ui->arcWeldingOutputWireFeedCheckBox, SIGNAL(clicked(bool)),this, SLOT(storeArcWeldingOutputWireFeedCheckBoxOnSelection(bool)));
    QObject::connect(ui->reviewBlockDiagramSettingsSaveToolButton, SIGNAL(clicked()),this, SLOT(saveReviewBlockDiagramSettingsOnClick()));
    QObject::connect(ui->reviewBlockDiagramSettingsLoadToolButton, SIGNAL(clicked()),this, SLOT(loadReviewBlockDiagramSettingsOnClick()));
    QObject::connect(ui->reviewBlockDiagramSettingsHomeToolButton, SIGNAL(clicked()),this, SLOT(gotoHomeWidgetOnClick()));
    QObject::connect(ui->adapParametersSettingsSaveToolButton, SIGNAL(clicked()),this, SLOT(saveReviewBlockDiagramSettingsOnClick()));
    QObject::connect(ui->adapParametersSettingsLoadToolButton, SIGNAL(clicked()),this, SLOT(loadReviewBlockDiagramSettingsOnClick()));
    QObject::connect(ui->adapParametersSettingsHomeToolButton, SIGNAL(clicked()),this, SLOT(gotoHomeWidgetOnClick()));
    QObject::connect(ui->arcWeldingOutputWireFeedFilterComboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(storeArcWeldingOutputWireFeedFilterOnChange(int)));
    QObject::connect(ui->arcWeldingOutputWireFeedNoMatchComboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(storeArcWeldingOutputWireFeedNoMatchOnChange(int)));
    QObject::connect(ui->arcWeldingOutputVoltageFilterComboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(storeArcWeldingOutputVoltageFilterOnChange(int)));
    QObject::connect(ui->arcWeldingOutputVoltageNoMatchComboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(storeArcWeldingOutputVoltageNoMatchOnChange(int)));
    QObject::connect(ui->arcWeldingOutputCurrentFilterComboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(storeArcWeldingOutputCurrentFilterOnChange(int)));
    QObject::connect(ui->arcWeldingOutputCurrentNoMatchComboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(storeArcWeldingOutputCurrentNoMatchOnChange(int)));
    QObject::connect(ui->arcWeldingOutputTravelSpeedFilterComboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(storeArcWeldingOutputTravelSpeedFilterOnChange(int)));
    QObject::connect(ui->arcWeldingOutputTravelSpeedNoMatchComboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(storeArcWeldingOutputTravelSpeedNoMatchOnChange(int)));
    QObject::connect(ui->finalPageEditToolButton, SIGNAL(clicked()),this, SLOT(enableBlockDiagramForEditing()));
    QObject::connect(ui->finalPageSaveToolButton, SIGNAL(clicked()),this, SLOT(saveBlockDiagramAfterEditing()));
    QObject::connect(ui->finalPageInputParametersSaveButton, SIGNAL(clicked()),this, SLOT(saveFinalPageInputWidgetData()));
    QObject::connect(ui->finalPageLookupTableButton, SIGNAL(clicked()),this, SLOT(showFinalPagelutWidget()));
    QObject::connect(ui->finalPageSaveLutBlockButton, SIGNAL(clicked()),this, SLOT(saveFinalPagelutWidgetData()));
    QObject::connect(ui->finalPageGenerateXMLButton, SIGNAL(clicked()),this, SLOT(callTinyXMLApiToGenerateAdapXML()));
    QObject::connect(ui->finalPageCloseScreenButton, SIGNAL(clicked()), this, SLOT(close()));
}
void AdapHmiMainWindow::initializeAllWidgets(){
    reviewBlockDiagramWeldingParametersWidgetState = new adaphmiwidgetstate();
    reviewBlockDiagramWeldingParametersWidgetState->setWidgetState(ui->reviewBlockDiagramWeldingParametersWidget,true);
    arcWeldingParametersWidgetState = new adaphmiwidgetstate();
    arcWeldingParametersWidgetState->setWidgetState(ui->arcWeldingParametersWidget,true);
    arcWeldingAdditionalParameterWidgetState = new adaphmiwidgetstate();
    arcWeldingAdditionalParameterWidgetState->setWidgetState(ui->arcWeldingAdditionalParameterWidget,true);
    reviewBlockDiagramSettingsWidgetState = new adaphmiwidgetstate();
    reviewBlockDiagramSettingsWidgetState->setWidgetState(ui->reviewBlockDiagramSettingsWidget,true);
    adapParametersSettingsWidgetState = new adaphmiwidgetstate();
    adapParametersSettingsWidgetState->setWidgetState(ui->adapParametersSettingsWidget,true);
    arcWeldingInputParametersVisionInputWidgetState = new adaphmiwidgetstate();
    arcWeldingInputParametersVisionInputWidgetState->setWidgetState(ui->arcWeldingInputParametersVisionInputWidget,true);
    arcWeldingInputParametersFilteringLevelWidgetState = new adaphmiwidgetstate();
    arcWeldingInputParametersFilteringLevelWidgetState->setWidgetState(ui->arcWeldingInputParametersFilteringLevelWidget,true);
    arcWeldingInputParametersNoMatchBehaviourWidgetState = new adaphmiwidgetstate();
    arcWeldingInputParametersNoMatchBehaviourWidgetState->setWidgetState(ui->arcWeldingInputParametersNoMatchBehaviourWidget,true);
    arcWeldingOutputParametersBodyWidgetState = new adaphmiwidgetstate();
    arcWeldingOutputParametersBodyWidgetState->setWidgetState(ui->arcWeldingOutputParametersBodyWidget,true);
    arcWeldingOutputAdditionalParameterWidgetState = new adaphmiwidgetstate();
    arcWeldingOutputAdditionalParameterWidgetState->setWidgetState(ui->arcWeldingOutputAdditionalParameterWidget,true);
    arcWeldingLookUpTableViewState = new adaphmiwidgetstate();
    arcWeldingLookUpTableViewState->setWidgetState(ui->arcWeldingLookUpTableView,true);
    arcWeldingLookUpTableState = new adaphmiwidgetstate();
    arcWeldingLookUpTableState->setWidgetState(ui->arcWeldingLookUpTableView,true);
    adapHmiLiveModeState = new adaphmiwidgetstate();
    adapHmiLiveModeState->setWidgetState(ui->adapHMIStackedWidget,true);
    ui->arcWeldingWireFeedLineEdit->setValidator(new QDoubleValidator(0, 1000, 2, this));
    ui->arcWeldingCurrentLineEdit->setValidator(new QDoubleValidator(0, 1000, 2, this));
    ui->arcWeldingVoltageLineEdit->setValidator(new QDoubleValidator(0, 1000, 2, this));
    ui->arcWeldingTravelSpeedLineEdit->setValidator(new QDoubleValidator(0, 1000, 2, this));
    ui->arcWeldingAmplitudeLineEdit->setValidator(new QDoubleValidator(0, 1000, 2, this));
    ui->arcWeldingFrequencyLineEdit->setValidator(new QDoubleValidator(0, 1000, 2, this));
    lutInputOutputMap.clear();
    finalPageInputWidgetState = new adaphmiwidgetstate();
    arcWeldingInputParametersVisionInputWidgetState->setWidgetState(ui->finalPageInputWidget,true);
    finalPageInputPushButton1State = new adaphmiwidgetstate();
    finalPageInputPushButton1State->setWidgetState(ui->finalPageInputGroupFrame,false);
    finalPageInputPushButton2State = new adaphmiwidgetstate();
    finalPageInputPushButton2State->setWidgetState(ui->finalPageInputGroupFrame,false);
    finalPageInputPushButton3State = new adaphmiwidgetstate();
    finalPageInputPushButton3State->setWidgetState(ui->finalPageInputGroupFrame,false);
    finalPageOutputPushButton1State = new adaphmiwidgetstate();
    finalPageOutputPushButton1State->setWidgetState(ui->finalPageOutputGroupFrame,false);
    finalPageOutputPushButton2State = new adaphmiwidgetstate();
    finalPageOutputPushButton2State->setWidgetState(ui->finalPageOutputGroupFrame,false);
    finalPageOutputPushButton3State = new adaphmiwidgetstate();
    finalPageOutputPushButton3State->setWidgetState(ui->finalPageOutputGroupFrame,false);
    finalPageOutput1WidgetState = new adaphmiwidgetstate();
    finalPageOutput1WidgetState->setWidgetState(ui->finalPageOutput1Widget,true);
    finalPageOutput2WidgetState = new adaphmiwidgetstate();
    finalPageOutput2WidgetState->setWidgetState(ui->finalPageOutput2Widget,true);
    finalPageOutput3WidgetState = new adaphmiwidgetstate();
    finalPageOutput3WidgetState->setWidgetState(ui->finalPageOutput3Widget,true);
    finalPageLookUpTableState = new adaphmiwidgetstate();
    finalPageLookUpTableState->setWidgetState(ui->finalPageLutBlockWidget,true);
    outputCount = 0;
}
void AdapHmiMainWindow::storeArcWeldingWireFeedLineEdit(){
    lutInputOutputMap.insert("INPUT_WIREFEED",ui->arcWeldingWireFeedLineEdit->text());
    ui->arcWeldingCurrentLineEdit->setEnabled(true);
    ui->arcWeldingCurrentLineEdit->setStyleSheet("background-color: rgb(255, 255, 255)");
    ui->arcWeldingCurrentLineEdit->setFocus();
}
void AdapHmiMainWindow::storeArcWeldingCurrentLineEdit(){
    lutInputOutputMap.insert("INPUT_CURRENT",ui->arcWeldingCurrentLineEdit->text());
    ui->arcWeldingVoltageLineEdit->setEnabled(true);
    ui->arcWeldingVoltageLineEdit->setStyleSheet("background-color: rgb(255, 255, 255)");
    ui->arcWeldingVoltageLineEdit->setFocus();
}
void AdapHmiMainWindow::storeArcWeldingVoltageLineEdit(){
    lutInputOutputMap.insert("INPUT_VOLTAGE",ui->arcWeldingVoltageLineEdit->text());
    ui->arcWeldingTravelSpeedLineEdit->setEnabled(true);
    ui->arcWeldingTravelSpeedLineEdit->setStyleSheet("background-color: rgb(255, 255, 255)");
    ui->arcWeldingTravelSpeedLineEdit->setFocus();
}
void AdapHmiMainWindow::storeArcWeldingTravelSpeedLineEdit(){
    lutInputOutputMap.insert("INPUT_TRAVELSPEED",ui->arcWeldingTravelSpeedLineEdit->text());
    ui->arcWeldingAdditionalParameterPushButton->setEnabled(true);
    ui->arcWeldingAdditionalParameterPushButton->setFocus();
}
void AdapHmiMainWindow::storeArcWeldingAmplitudeLineEdit(){
    lutInputOutputMap.insert("INPUT_AMPLITUDE",ui->arcWeldingAmplitudeLineEdit->text());
    ui->arcWeldingFrequencyLineEdit->setEnabled(true);
    ui->arcWeldingFrequencyLineEdit->setStyleSheet("background-color: rgb(255, 255, 255)");
    ui->arcWeldingFrequencyLineEdit->setFocus();
}
void AdapHmiMainWindow::storeArcWeldingFrequencyLineEdit(){
    lutInputOutputMap.insert("INPUT_FREQUENCY",ui->arcWeldingFrequencyLineEdit->text());
    ui->reviewBlockDiagramApplyButton->setFocus();
}
void AdapHmiMainWindow::storeArcWeldingInputParametersAreaCheckBoxOnSelection(bool checked){
    if(!arcWeldingLookUpTableState->getWidgetState()){
         arcWeldingLookUpTableState->setWidgetState(ui->arcWeldingLookUpTableView,true);
    }
    if(!adapHmiLiveModeState->getWidgetState()){
        adapHmiLiveModeState->setWidgetState(ui->adapHMIStackedWidget,true);
    }
    if(checked){
        lutInputOutputMap.insert("IP_AREA","YES");
        ui->arcWeldingInputParametersGapCheckBox->setEnabled(false);
        ui->arcWeldingInputParametersMismatchCheckBox->setEnabled(false);
    } else {
        lutInputOutputMap.remove("IP_AREA");
        ui->arcWeldingInputParametersGapCheckBox->setEnabled(true);
        ui->arcWeldingInputParametersMismatchCheckBox->setEnabled(true);
    }
}
void AdapHmiMainWindow::storeArcWeldingInputParametersGapCheckBoxOnSelection(bool checked){
    if(!arcWeldingLookUpTableState->getWidgetState()){
         arcWeldingLookUpTableState->setWidgetState(ui->arcWeldingLookUpTableView,true);
    }
    if(!adapHmiLiveModeState->getWidgetState()){
        adapHmiLiveModeState->setWidgetState(ui->adapHMIStackedWidget,true);
    }
    if(checked){
        lutInputOutputMap.insert("IP_GAP","YES");
        ui->arcWeldingInputParametersAreaCheckBox->setEnabled(false);
        ui->arcWeldingInputParametersMismatchCheckBox->setEnabled(false);
    } else {
        lutInputOutputMap.remove("IP_GAP");
        ui->arcWeldingInputParametersAreaCheckBox->setEnabled(true);
        ui->arcWeldingInputParametersMismatchCheckBox->setEnabled(true);
    }
}
void AdapHmiMainWindow::storeArcWeldingInputParametersMismatchCheckBoxOnSelection(bool checked){
    if(!arcWeldingLookUpTableState->getWidgetState()){
         arcWeldingLookUpTableState->setWidgetState(ui->arcWeldingLookUpTableView,true);
    }
    if(!adapHmiLiveModeState->getWidgetState()){
        adapHmiLiveModeState->setWidgetState(ui->adapHMIStackedWidget,true);
    }
    if(checked){
        lutInputOutputMap.insert("IP_MISMATCH","YES");
        ui->arcWeldingInputParametersAreaCheckBox->setEnabled(false);
        ui->arcWeldingInputParametersGapCheckBox->setEnabled(false);
    } else {
        lutInputOutputMap.remove("IP_MISMATCH");
        ui->arcWeldingInputParametersAreaCheckBox->setEnabled(true);
        ui->arcWeldingInputParametersGapCheckBox->setEnabled(true);
    }
}
void AdapHmiMainWindow::storeArcWeldingInputParametersMaintainOutputBehaviourRadioButtonOnSelection(bool checked){
    if(checked){
        lutInputOutputMap.insert("INPUT_IF_NOMATCH","MAINTAINOUTPUT");
    }
}
void AdapHmiMainWindow::storeArcWeldingInputParametersResetToNormalBehaviourRadioButtonOnSelection(bool checked){
    if(checked){
        lutInputOutputMap.insert("INPUT_IF_NOMATCH","RESETTONORMAL");
    }
}
void AdapHmiMainWindow::storeArcWeldingInputParametersHighFilteringLevelRadioButtonOnSelection(bool checked){
    if(checked){
        lutInputOutputMap.insert("INPUT_FILTER_LEVEL","HIGH");
    }
}
void AdapHmiMainWindow::storeArcWeldingInputParametersMediumFilteringLevelRadioButtonOnSelection(bool checked){
    if(checked){
        lutInputOutputMap.insert("INPUT_FILTER_LEVEL","MEDIUM");
    }
}
void AdapHmiMainWindow::storeArcWeldingInputParametersNoneFilteringLevelRadioButtonOnSelection(bool checked){
    if(checked){
        lutInputOutputMap.insert("INPUT_FILTER_LEVEL","NONE");
    }
}
void AdapHmiMainWindow::storeArcWeldingInputParametersSmallFilteringLevelRadioButtonOnSelection(bool checked){
    if(checked){
        lutInputOutputMap.insert("INPUT_FILTER_LEVEL","LOW");
    }
}
void AdapHmiMainWindow::storeArcWeldingOutputAmplitudeCheckBoxOnSelection(bool checked){
    if(!arcWeldingLookUpTableState->getWidgetState()){
         arcWeldingLookUpTableState->setWidgetState(ui->arcWeldingLookUpTableView,true);
    }
    if(!adapHmiLiveModeState->getWidgetState()){
        adapHmiLiveModeState->setWidgetState(ui->adapHMIStackedWidget,true);
    }
    if(checked){
        lutInputOutputMap.insert("AO_AMPLITUDE","YES");
    } else {
        lutInputOutputMap.remove("AO_AMPLITUDE");
    }
}
void AdapHmiMainWindow::storeArcWeldingOutputFrequencyCheckBoxOnSelection(bool checked){
    if(!arcWeldingLookUpTableState->getWidgetState()){
         arcWeldingLookUpTableState->setWidgetState(ui->arcWeldingLookUpTableView,true);
    }
    if(!adapHmiLiveModeState->getWidgetState()){
        adapHmiLiveModeState->setWidgetState(ui->adapHMIStackedWidget,true);
    }
    if(checked){
        lutInputOutputMap.insert("AO_FREQUENCY","YES");
    } else {
        lutInputOutputMap.remove("AO_FREQUENCY");
    }
}
void AdapHmiMainWindow::storeArcWeldingOutputCurrentCheckBoxOnSelection(bool checked){
    if(!arcWeldingLookUpTableState->getWidgetState()){
         arcWeldingLookUpTableState->setWidgetState(ui->arcWeldingLookUpTableView,true);
    }
    if(!adapHmiLiveModeState->getWidgetState()){
        adapHmiLiveModeState->setWidgetState(ui->adapHMIStackedWidget,true);
    }
    if(checked){
        lutInputOutputMap.insert("OP_CURRENT","YES");
        ui->arcWeldingOutputCurrentFilterComboBox->setEnabled(true);
        ui->arcWeldingOutputCurrentNoMatchComboBox->setEnabled(true);
        AdapHmiMainWindow::enableDisableOutPutCheckBoxes(1);
    } else {
        lutInputOutputMap.remove("OP_CURRENT");
        ui->arcWeldingOutputCurrentFilterComboBox->setCurrentIndex(0);
        ui->arcWeldingOutputCurrentNoMatchComboBox->setCurrentIndex(0);
        ui->arcWeldingOutputCurrentFilterComboBox->setEnabled(false);
        ui->arcWeldingOutputCurrentNoMatchComboBox->setEnabled(false);
        AdapHmiMainWindow::enableDisableOutPutCheckBoxes(0);
    }
}
void AdapHmiMainWindow::storeArcWeldingOutputTravelSpeedCheckBoxOnSelection(bool checked){
    if(!arcWeldingLookUpTableState->getWidgetState()){
         arcWeldingLookUpTableState->setWidgetState(ui->arcWeldingLookUpTableView,true);
    }
    if(!adapHmiLiveModeState->getWidgetState()){
        adapHmiLiveModeState->setWidgetState(ui->adapHMIStackedWidget,true);
    }
    if(checked){
        lutInputOutputMap.insert("OP_TRAVELSPEED","YES");
        ui->arcWeldingOutputTravelSpeedFilterComboBox->setEnabled(true);
        ui->arcWeldingOutputTravelSpeedNoMatchComboBox->setEnabled(true);
        AdapHmiMainWindow::enableDisableOutPutCheckBoxes(1);
    } else {
        lutInputOutputMap.remove("OP_TRAVELSPEED");
        ui->arcWeldingOutputTravelSpeedFilterComboBox->setCurrentIndex(0);
        ui->arcWeldingOutputTravelSpeedNoMatchComboBox->setCurrentIndex(0);
        ui->arcWeldingOutputTravelSpeedFilterComboBox->setEnabled(false);
        ui->arcWeldingOutputTravelSpeedNoMatchComboBox->setEnabled(false);
        AdapHmiMainWindow::enableDisableOutPutCheckBoxes(0);
    }
}
void AdapHmiMainWindow::storeArcWeldingOutputVoltageCheckBoxOnSelection(bool checked){
    if(!arcWeldingLookUpTableState->getWidgetState()){
         arcWeldingLookUpTableState->setWidgetState(ui->arcWeldingLookUpTableView,true);
    }
    if(!adapHmiLiveModeState->getWidgetState()){
        adapHmiLiveModeState->setWidgetState(ui->adapHMIStackedWidget,true);
    }
    if(checked){
        lutInputOutputMap.insert("OP_VOLTAGE","YES");
        ui->arcWeldingOutputVoltageFilterComboBox->setEnabled(true);
        ui->arcWeldingOutputVoltageNoMatchComboBox->setEnabled(true);
        AdapHmiMainWindow::enableDisableOutPutCheckBoxes(1);
    } else {
        lutInputOutputMap.remove("OP_VOLTAGE");
        ui->arcWeldingOutputVoltageFilterComboBox->setCurrentIndex(0);
        ui->arcWeldingOutputVoltageNoMatchComboBox->setCurrentIndex(0);
        ui->arcWeldingOutputVoltageFilterComboBox->setEnabled(false);
        ui->arcWeldingOutputVoltageNoMatchComboBox->setEnabled(false);
        AdapHmiMainWindow::enableDisableOutPutCheckBoxes(0);
    }
}
void AdapHmiMainWindow::storeArcWeldingOutputWireFeedCheckBoxOnSelection(bool checked){
    if(!arcWeldingLookUpTableState->getWidgetState()){
         arcWeldingLookUpTableState->setWidgetState(ui->arcWeldingLookUpTableView,true);         
    }
    if(!adapHmiLiveModeState->getWidgetState()){
        adapHmiLiveModeState->setWidgetState(ui->adapHMIStackedWidget,true);
    }
    if(checked){
        lutInputOutputMap.insert("OP_WIREFEED","YES");
        ui->arcWeldingOutputWireFeedFilterComboBox->setEnabled(true);
        ui->arcWeldingOutputWireFeedNoMatchComboBox->setEnabled(true);
        AdapHmiMainWindow::enableDisableOutPutCheckBoxes(1);
    } else {
        lutInputOutputMap.remove("OP_WIREFEED");
        ui->arcWeldingOutputWireFeedFilterComboBox->setCurrentIndex(0);
        ui->arcWeldingOutputWireFeedNoMatchComboBox->setCurrentIndex(0);
        ui->arcWeldingOutputWireFeedFilterComboBox->setEnabled(false);
        ui->arcWeldingOutputWireFeedNoMatchComboBox->setEnabled(false);
        AdapHmiMainWindow::enableDisableOutPutCheckBoxes(0);
    }
}
void AdapHmiMainWindow::createlutTableBasedOnInputOutputParameters(){
    int columnCount=0;
    foreach(QString key, lutInputOutputMap.keys()){
        if(key.contains("IP_") || key.contains("OP_")){
            columnCount++;
        }
    }
    if(adapHmiLiveModeState->getWidgetState()){
        lutModel = new QStandardItemModel(3,columnCount,this);
    }
    columnCount=0;
    foreach(QString key, lutInputOutputMap.keys()){
        if(key.contains("IP_") || key.contains("OP_")){
            if(lutInputOutputMap.value(key) == "YES"){
                lutModel->setHorizontalHeaderItem(columnCount, new QStandardItem(key.mid(3,key.length()-3)));
            }
            columnCount++;
        }
    }
    lutModel->setHorizontalHeaderItem(columnCount, new QStandardItem(QIcon(":adapHMIResources/images/Ico_Delete.png"),""));
    if(adapHmiLiveModeState->getWidgetState()){
        for (int rows=0;rows<3;rows++){
            for (int columns=0;columns<=columnCount;columns++){
                    lutModel->setItem(rows,columns,new QStandardItem("0"));
                    if(columns == columnCount){
                        lutModel->setItem(rows,columns,new QStandardItem(QIcon(":adapHMIResources/images/Ico_Delete.png"),""));
                    }
            }            
        }
    }
    ui->arcWeldingLookUpTableView->setModel(lutModel);
    ui->arcWeldingLookUpTableView->horizontalHeader()->setVisible(true);
    ui->arcWeldingLookUpTableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->arcWeldingLookUpTableView->horizontalHeader()->setClickable(false);
    ui->arcWeldingLookUpTableView->verticalHeader()->setVisible(false);
    ui->arcWeldingLookUpTableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->arcWeldingLookUpTableView->setTabKeyNavigation(false);
    QObject::connect(ui->arcWeldingLookUpTableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),SLOT(insertArcWeldingLookUpTableViewOnSelectionChange(const QItemSelection &, const QItemSelection &)));
    QObject::connect(ui->adapParametersApplyButton,SIGNAL(clicked()),this,SLOT(saveArcWeldingLookUpTableData()));
}
void AdapHmiMainWindow::insertArcWeldingLookUpTableViewOnSelectionChange(const QItemSelection &, const QItemSelection &){
        ui->arcWeldingLookUpTableView->setTabKeyNavigation(false);
        QModelIndexList selectedRowInLutTableIndexes = ui->arcWeldingLookUpTableView->selectionModel()->selection().indexes();
        for (int i = 0; i < selectedRowInLutTableIndexes.count(); ++i){
            QModelIndex lutIndex = selectedRowInLutTableIndexes.at(i);
            QAbstractItemModel *previousLutModel = ui->arcWeldingLookUpTableView->model();
            if(lutIndex.row() == previousLutModel->rowCount()-1 && lutIndex.column() == 0){
                previousLutModel->insertRow(previousLutModel->rowCount());
                previousLutModel->setData(previousLutModel->index(lutIndex.row()+1,previousLutModel->columnCount()-1,QModelIndex()),QIcon(":adapHMIResources/images/Ico_Delete.png"),Qt::DecorationRole);
            }
            if(lutIndex.column() == previousLutModel->columnCount()-1 && previousLutModel->rowCount() > 1 ){
                previousLutModel->removeRows(lutIndex.row(),1);
            }
        }
}
void AdapHmiMainWindow::saveArcWeldingLookUpTableData(){
    QAbstractItemModel *tempLutModel = ui->arcWeldingLookUpTableView->model();
    int row = tempLutModel->rowCount();
    int col = tempLutModel->columnCount();
    for (int i = 0; i < row ; i++){
        for (int j = 0; j < col - 1; j++){
            if(!tempLutModel->data(tempLutModel->index(i,j), Qt::DisplayRole).toString().isEmpty()){
                lutMap.insert(new adaphmisavelookuptable(i,j),tempLutModel->data(tempLutModel->index(i,j), Qt::DisplayRole).toString());
            }
        }
    }
}
void AdapHmiMainWindow::saveReviewBlockDiagramSettingsOnClick(){
    if(!lutInputOutputMap.contains("INPUT_FREQUENCY") && ui->arcWeldingFrequencyLineEdit->isEnabled()){
        lutInputOutputMap.insert("INPUT_FREQUENCY",ui->arcWeldingFrequencyLineEdit->text());
    }
    QString saveSettingsToFile = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),tr("Settings Files (*.ini);;Text Files (*.txt)"));
    saveAdapHmiUserInterfaceState = new adaphmisaveload(saveSettingsToFile);
    saveAdapHmiUserInterfaceState->saveSettings("UIGeometry",saveGeometry(),"ADAPHMI");
    saveAdapHmiUserInterfaceState->saveSettings("UIState",saveState(),"ADAPHMI");
    saveAdapHmiUserInterfaceState->saveSettingsMap(lutInputOutputMap,"LUTINPUTOUTPUT");
    if(ui->arcWeldingLookUpTableView->model() != NULL){
        AdapHmiMainWindow::saveArcWeldingLookUpTableData();
        saveAdapHmiUserInterfaceState->savelookupTableMap(lutMap,"LOOKUPTABLE");
    }
}
void AdapHmiMainWindow::loadReviewBlockDiagramSettingsOnClick(){
    QString loadSettingsFromFile = QFileDialog::getOpenFileName(this, tr("Save File"), QString(),tr("Settings Files (*.ini);;Text Files (*.txt)"));
    loadAdapHmiUserInterfaceState = new adaphmisaveload(loadSettingsFromFile);
    QByteArray geometryData = loadAdapHmiUserInterfaceState->loadSettings("UIGeometry","ADAPHMI",QVariant()).toByteArray();
    QByteArray stateData = loadAdapHmiUserInterfaceState->loadSettings("UIState","ADAPHMI",QVariant()).toByteArray();
    AdapHmiMainWindow::restoreGeometry(geometryData);
    AdapHmiMainWindow::restoreState(stateData);
    lutInputOutputMap = loadAdapHmiUserInterfaceState->loadSettingsMap("LUTINPUTOUTPUT");
    AdapHmiMainWindow::loadlutInputOutputMapFromSettings();
    lutMap = loadAdapHmiUserInterfaceState->loadlookupTableMap("LOOKUPTABLE");
    AdapHmiMainWindow::loadlookuptableMapFromSettings();
}
void AdapHmiMainWindow::gotoHomeWidgetOnClick(){
    ui->adapHMIStackedWidget->setCurrentIndex(0);
}
void AdapHmiMainWindow::loadlutInputOutputMapFromSettings(){
    outputCount=0;
    if(lutInputOutputMap.contains("INPUT_WIREFEED")){
        ui->arcWeldingWireFeedLineEdit->setText(lutInputOutputMap.value("INPUT_WIREFEED"));
        ui->arcWeldingCurrentLineEdit->setEnabled(true);
        ui->arcWeldingCurrentLineEdit->setStyleSheet("background-color: rgb(255, 255, 255)");
        ui->arcWeldingCurrentLineEdit->setFocus();
    }
    if(lutInputOutputMap.contains("INPUT_CURRENT")){
        ui->arcWeldingCurrentLineEdit->setText(lutInputOutputMap.value("INPUT_CURRENT"));
        ui->arcWeldingVoltageLineEdit->setEnabled(true);
        ui->arcWeldingVoltageLineEdit->setStyleSheet("background-color: rgb(255, 255, 255)");
        ui->arcWeldingVoltageLineEdit->setFocus();
    }
    if(lutInputOutputMap.contains("INPUT_VOLTAGE")){
        ui->arcWeldingVoltageLineEdit->setText(lutInputOutputMap.value("INPUT_VOLTAGE"));
        ui->arcWeldingTravelSpeedLineEdit->setEnabled(true);
        ui->arcWeldingTravelSpeedLineEdit->setStyleSheet("background-color: rgb(255, 255, 255)");
        ui->arcWeldingTravelSpeedLineEdit->setFocus();
    }
    if(lutInputOutputMap.contains("INPUT_TRAVELSPEED")){
        ui->arcWeldingTravelSpeedLineEdit->setText(lutInputOutputMap.value("INPUT_TRAVELSPEED"));
        ui->arcWeldingAdditionalParameterPushButton->setEnabled(true);
        ui->arcWeldingAdditionalParameterPushButton->setFocus();
    }
    if(lutInputOutputMap.contains("INPUT_WEAVINGREQ")){
        if(lutInputOutputMap.value("INPUT_WEAVINGREQ") == "YES"){
            ui->arcWeldingWeavingReqYesRadioButton->setChecked(true);
            ui->arcWeldingAmplitudeLineEdit->setEnabled(true);
            ui->arcWeldingAmplitudeLineEdit->setStyleSheet("background-color: rgb(255, 255, 255)");
            ui->arcWeldingAmplitudeLineEdit->setFocus();
            AdapHmiMainWindow::retractExpandGivenWidget(10,214,271,81,arcWeldingOutputAdditionalParameterWidgetState,ui->arcWeldingOutputAdditionalParameterWidget);
        } else {
            ui->arcWeldingWeavingReqNoRadioButton->setChecked(true);
        }
    }
    if(lutInputOutputMap.contains("INPUT_AMPLITUDE")){
        ui->arcWeldingAmplitudeLineEdit->setText(lutInputOutputMap.value("INPUT_AMPLITUDE"));
        ui->arcWeldingFrequencyLineEdit->setEnabled(true);
        ui->arcWeldingFrequencyLineEdit->setStyleSheet("background-color: rgb(255, 255, 255)");
        ui->arcWeldingFrequencyLineEdit->setFocus();
    }
    if(lutInputOutputMap.contains("INPUT_FREQUENCY")){
        ui->arcWeldingFrequencyLineEdit->setText(lutInputOutputMap.value("INPUT_FREQUENCY"));
        ui->reviewBlockDiagramApplyButton->setFocus();
    }
    if(lutInputOutputMap.contains("IP_AREA")){
        ui->arcWeldingInputParametersAreaCheckBox->setChecked(true);
        ui->arcWeldingInputParametersGapCheckBox->setEnabled(false);
        ui->arcWeldingInputParametersMismatchCheckBox->setEnabled(false);
    }
    if(lutInputOutputMap.contains("IP_GAP")){
        ui->arcWeldingInputParametersGapCheckBox->setChecked(true);
        ui->arcWeldingInputParametersAreaCheckBox->setEnabled(false);
        ui->arcWeldingInputParametersMismatchCheckBox->setEnabled(false);
    }
    if(lutInputOutputMap.contains("IP_MISMATCH")){
        ui->arcWeldingInputParametersMismatchCheckBox->setChecked(true);
        ui->arcWeldingInputParametersAreaCheckBox->setEnabled(false);
        ui->arcWeldingInputParametersGapCheckBox->setEnabled(false);
    }
    if(lutInputOutputMap.contains("INPUT_IF_NOMATCH")){
        if(lutInputOutputMap.value("INPUT_IF_NOMATCH") == "MAINTAINOUTPUT"){
            ui->arcWeldingInputParametersResetToNormalBehaviourRadioButton->setChecked(true);
        } else if(lutInputOutputMap.value("INPUT_IF_NOMATCH") == "RESETTONORMAL"){
            ui->arcWeldingInputParametersMaintainOutputBehaviourRadioButton->setChecked(true);
        }
    }
    if(lutInputOutputMap.contains("INPUT_FILTER_LEVEL")){
        if(lutInputOutputMap.value("INPUT_FILTER_LEVEL") == "HIGH"){
            ui->arcWeldingInputParametersHighFilteringLevelRadioButton->setChecked(true);
        } else if(lutInputOutputMap.value("INPUT_FILTER_LEVEL") == "MEDIUM"){
            ui->arcWeldingInputParametersMediumFilteringLevelRadioButton->setChecked(true);
        } else if(lutInputOutputMap.value("INPUT_FILTER_LEVEL") == "NONE"){
            ui->arcWeldingInputParametersNoneFilteringLevelRadioButton->setChecked(true);
        } else if(lutInputOutputMap.value("INPUT_FILTER_LEVEL") == "LOW"){
            ui->arcWeldingInputParametersSmallFilteringLevelRadioButton->setChecked(true);
        }
    }
    if(lutInputOutputMap.contains("AO_AMPLITUDE")){
        ui->arcWeldingOutputAmplitudeCheckBox->setChecked(true);
    }
    if(lutInputOutputMap.contains("AO_FREQUENCY")){
        ui->arcWeldingOutputFrequencyCheckBox->setChecked(true);
    }
    if(lutInputOutputMap.contains("OP_WIREFEED")){
        ui->arcWeldingOutputWireFeedCheckBox->setChecked(true);
        ui->arcWeldingOutputWireFeedFilterComboBox->setEnabled(true);
        ui->arcWeldingOutputWireFeedNoMatchComboBox->setEnabled(true);
        if(lutInputOutputMap.contains("OUTPUT_WIREFEED_FILTER_LEVEL")){
            ui->arcWeldingOutputWireFeedFilterComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_WIREFEED_FILTER_LEVEL").toInt());
        }
        if(lutInputOutputMap.contains("OUTPUT_WIREFEED_IF_NOMATCH")){
            ui->arcWeldingOutputWireFeedNoMatchComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_WIREFEED_IF_NOMATCH").toInt());
        }
        AdapHmiMainWindow::enableDisableOutPutCheckBoxes(1);
    }
    if(lutInputOutputMap.contains("OP_CURRENT")){
        ui->arcWeldingOutputCurrentCheckBox->setChecked(true);
        ui->arcWeldingOutputCurrentFilterComboBox->setEnabled(true);
        ui->arcWeldingOutputCurrentNoMatchComboBox->setEnabled(true);
        if(lutInputOutputMap.contains("OUTPUT_CURRENT_FILTER_LEVEL")){
            ui->arcWeldingOutputCurrentFilterComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_CURRENT_FILTER_LEVEL").toInt());
        }
        if(lutInputOutputMap.contains("OUTPUT_CURRENT_IF_NOMATCH")){
            ui->arcWeldingOutputCurrentNoMatchComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_CURRENT_IF_NOMATCH").toInt());
        }
        AdapHmiMainWindow::enableDisableOutPutCheckBoxes(1);
    }
    if(lutInputOutputMap.contains("OP_VOLTAGE")){
        ui->arcWeldingOutputVoltageCheckBox->setChecked(true);
        ui->arcWeldingOutputVoltageFilterComboBox->setEnabled(true);
        ui->arcWeldingOutputVoltageNoMatchComboBox->setEnabled(true);
        if(lutInputOutputMap.contains("OUTPUT_VOLTAGE_FILTER_LEVEL")){
            ui->arcWeldingOutputVoltageFilterComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_VOLTAGE_FILTER_LEVEL").toInt());
        }
        if(lutInputOutputMap.contains("OUTPUT_VOLTAGE_IF_NOMATCH")){
            ui->arcWeldingOutputVoltageNoMatchComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_VOLTAGE_IF_NOMATCH").toInt());
        }
        AdapHmiMainWindow::enableDisableOutPutCheckBoxes(1);
    }
    if(lutInputOutputMap.contains("OP_TRAVELSPEED")){
        ui->arcWeldingOutputTravelSpeedCheckBox->setChecked(true);
        ui->arcWeldingOutputTravelSpeedFilterComboBox->setEnabled(true);
        ui->arcWeldingOutputTravelSpeedNoMatchComboBox->setEnabled(true);
        if(lutInputOutputMap.contains("OUTPUT_TRAVELSPEED_FILTER_LEVEL")){
            ui->arcWeldingOutputTravelSpeedFilterComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_TRAVELSPEED_FILTER_LEVEL").toInt());
        }
        if(lutInputOutputMap.contains("OUTPUT_TRAVELSPEED_IF_NOMATCH")){
            ui->arcWeldingOutputTravelSpeedNoMatchComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_TRAVELSPEED_IF_NOMATCH").toInt());
        }
        AdapHmiMainWindow::enableDisableOutPutCheckBoxes(1);
    }
}
void AdapHmiMainWindow::loadlookuptableMapFromSettings(){
   adapHmiLiveModeState->setWidgetState(ui->adapHMIStackedWidget,false);
    if (lutMap.count() > 0){
        lutModel = new QStandardItemModel(this);
        QSet<int> rowData;
        QSet<int> colData;
        foreach( adaphmisavelookuptable* key, lutMap.keys()){
            rowData.insert(key->getRow());
            colData.insert(key->getCol());
        }
        foreach( adaphmisavelookuptable* key, lutMap.keys()){
          lutModel->setItem(key->getRow(),key->getCol(),new QStandardItem(lutMap.value(key)));
        }
        for (int i=0; i < rowData.count(); i++){
            lutModel->setItem(i,colData.count(),new QStandardItem(QIcon(":adapHMIResources/images/Ico_Delete.png"),""));
        }
    }
    AdapHmiMainWindow::createlutTableBasedOnInputOutputParameters();
}
void AdapHmiMainWindow::createBlockDiagramFromQMap(){
    int numberOfOutputBlock=0;
    int numberOfInputBlock=0;
    foreach(QString key, lutInputOutputMap.keys()){
        if(key.contains("OP_")){
            numberOfOutputBlock++;
        } else {
            if(key.contains("IP_")){
                numberOfInputBlock++;
            }
        }
    }
    if(lutInputOutputMap.contains("INPUT_IF_NOMATCH")){
        if(lutInputOutputMap.value("INPUT_IF_NOMATCH") == "MAINTAINOUTPUT"){
            ui->finalPageInputParametersMaintainOutputBehaviourRadioButton->setChecked(true);
        } else if(lutInputOutputMap.value("INPUT_IF_NOMATCH") == "RESETTONORMAL"){
            ui->finalPageInputParametersResetToNormalBehaviourRadioButton->setChecked(true);
        }
    }
    if(lutInputOutputMap.contains("INPUT_FILTER_LEVEL")){
        if(lutInputOutputMap.value("INPUT_FILTER_LEVEL") == "HIGH"){
            ui->finalPageInputParametersHighFilteringLevelRadioButton->setChecked(true);
        } else if(lutInputOutputMap.value("INPUT_FILTER_LEVEL") == "MEDIUM"){
            ui->finalPageInputParametersMediumFilteringLevelRadioButton->setChecked(true);
        } else if(lutInputOutputMap.value("INPUT_FILTER_LEVEL") == "NONE"){
            ui->finalPageInputParametersNoneFilteringLevelRadioButton->setChecked(true);
        } else if(lutInputOutputMap.value("INPUT_FILTER_LEVEL") == "LOW"){
            ui->finalPageInputParametersSmallFilteringLevelRadioButton->setChecked(true);
        }
    }
    QMap<QString,QString> inputKeyValuePair;
    if(lutInputOutputMap.contains("IP_AREA")){
        inputKeyValuePair.insert("AREA","AREA");
        ui->finalPageInputParametersAreaCheckBox->setChecked(true);
        ui->finalPageInputParametersAreaCheckBox->setEnabled(false);
        ui->finalPageInputParametersGapCheckBox->setEnabled(false);
        ui->finalPageInputParametersMismatchCheckBox->setEnabled(false);
    }
    if(lutInputOutputMap.contains("IP_GAP")){
        inputKeyValuePair.insert("GAP","GAP");
        ui->finalPageInputParametersGapCheckBox->setChecked(true);
        ui->finalPageInputParametersGapCheckBox->setEnabled(false);
        ui->finalPageInputParametersAreaCheckBox->setEnabled(false);
        ui->finalPageInputParametersMismatchCheckBox->setEnabled(false);
    }
    if(lutInputOutputMap.contains("IP_MISMATCH")){
        inputKeyValuePair.insert("MISMATCH","MISM");
        ui->finalPageInputParametersMismatchCheckBox->setChecked(true);
        ui->finalPageInputParametersMismatchCheckBox->setEnabled(false);
        ui->finalPageInputParametersAreaCheckBox->setEnabled(false);
        ui->finalPageInputParametersGapCheckBox->setEnabled(false);

    }
    QMap<QString,QString> outputKeyValuePair;
    if(lutInputOutputMap.contains("OP_CURRENT")){
        outputKeyValuePair.insert("I",lutInputOutputMap.value("INPUT_CURRENT"));
    }
    if(lutInputOutputMap.contains("OP_VOLTAGE")){
        outputKeyValuePair.insert("V",lutInputOutputMap.value("INPUT_VOLTAGE"));
    }
    if(lutInputOutputMap.contains("OP_TRAVELSPEED")){
        outputKeyValuePair.insert("TRAVEL",lutInputOutputMap.value("INPUT_TRAVELSPEED"));
    }
    if(lutInputOutputMap.contains("OP_WIREFEED")){
        outputKeyValuePair.insert("WIRE",lutInputOutputMap.value("INPUT_WIREFEED"));
    }
    AdapHmiMainWindow::createFinalPageInputBlocks(numberOfInputBlock, inputKeyValuePair);
    AdapHmiMainWindow::createFinalPageOutputBlocks(numberOfOutputBlock,outputKeyValuePair);
}
void AdapHmiMainWindow::storeArcWeldingOutputWireFeedFilterOnChange(int index){
    lutInputOutputMap.insert("OUTPUT_WIREFEED_FILTER_LEVEL",QString::number(index));
}
void AdapHmiMainWindow::storeArcWeldingOutputWireFeedNoMatchOnChange(int index){
    lutInputOutputMap.insert("OUTPUT_WIREFEED_IF_NOMATCH",QString::number(index));
}
void AdapHmiMainWindow::storeArcWeldingOutputVoltageFilterOnChange(int index){
    lutInputOutputMap.insert("OUTPUT_VOLTAGE_FILTER_LEVEL",QString::number(index));
}
void AdapHmiMainWindow::storeArcWeldingOutputVoltageNoMatchOnChange(int index){
    lutInputOutputMap.insert("OUTPUT_VOLTAGE_IF_NOMATCH",QString::number(index));
}
void AdapHmiMainWindow::storeArcWeldingOutputCurrentFilterOnChange(int index){
    lutInputOutputMap.insert("OUTPUT_CURRENT_FILTER_LEVEL",QString::number(index));
}
void AdapHmiMainWindow::storeArcWeldingOutputCurrentNoMatchOnChange(int index){
    lutInputOutputMap.insert("OUTPUT_CURRENT_IF_NOMATCH",QString::number(index));
}
void AdapHmiMainWindow::storeArcWeldingOutputTravelSpeedFilterOnChange(int index){
    lutInputOutputMap.insert("OUTPUT_TRAVELSPEED_FILTER_LEVEL",QString::number(index));
}
void AdapHmiMainWindow::storeArcWeldingOutputTravelSpeedNoMatchOnChange(int index){
    lutInputOutputMap.insert("OUTPUT_TRAVELSPEED_IF_NOMATCH",QString::number(index));
}
void AdapHmiMainWindow::enableBlockDiagramForEditing(){
    ui->finalPageLookupTableButton->setEnabled(true);
    ui->finalPageGenerateXMLButton->setEnabled(false);
    if(finalPageOutputPushButton1State->getWidgetState()){
        finalPageOutputPushButton1->setEnabled(true);
        ui->finalPageOutput1FilterComboBox->setEnabled(true);
        ui->finalPageOutput1NoMatchComboBox->setEnabled(true);
    }
    if(finalPageOutputPushButton2State->getWidgetState()){
        finalPageOutputPushButton2->setEnabled(true);
        ui->finalPageOutput2FilterComboBox->setEnabled(true);
        ui->finalPageOutput2NoMatchComboBox->setEnabled(true);
    }
    if(finalPageOutputPushButton3State->getWidgetState()){
        finalPageOutputPushButton3->setEnabled(true);
        ui->finalPageOutput3FilterComboBox->setEnabled(true);
        ui->finalPageOutput3NoMatchComboBox->setEnabled(true);
    }
    if(finalPageInputPushButton1State->getWidgetState()){
        finalPageInputPushButton1->setEnabled(true);
    }
    if(finalPageInputPushButton2State->getWidgetState()){
        finalPageInputPushButton2->setEnabled(true);
    }
    if(finalPageInputPushButton3State->getWidgetState()){
        finalPageInputPushButton3->setEnabled(true);
    }
    //ui->finalPageEditModeWidget->setStyleSheet("background-color: qradialgradient(spread:pad, cx:0.5, cy:0.499909, radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(111, 111, 111, 255), stop:1 rgba(178, 178, 178, 255));");
    ui->finalPageEditModeWidget->setStyleSheet("background-color: rgb(203, 203, 203);");
}
void AdapHmiMainWindow::saveBlockDiagramAfterEditing(){
    ui->finalPageLookupTableButton->setEnabled(false);
    ui->finalPageGenerateXMLButton->setEnabled(true);
    if(finalPageInputPushButton1State->getWidgetState()){
        finalPageInputPushButton1->setEnabled(false);
    }
    if(finalPageInputPushButton2State->getWidgetState()){
        finalPageInputPushButton2->setEnabled(false);
    }
    if(finalPageInputPushButton3State->getWidgetState()){
        finalPageInputPushButton3->setEnabled(false);
    }
    if(finalPageOutputPushButton1State->getWidgetState()){
        finalPageOutputPushButton1->setEnabled(false);
    }
    if(finalPageOutputPushButton2State->getWidgetState()){
        finalPageOutputPushButton2->setEnabled(false);
    }
    if(finalPageOutputPushButton3State->getWidgetState()){
        finalPageOutputPushButton3->setEnabled(false);
    }
    ui->finalPageEditModeWidget->setStyleSheet("background-color: none;");
    if(!finalPageInputWidgetState->getWidgetState()){
        AdapHmiMainWindow::retractExpandGivenWidget(70,30,311,455,finalPageInputWidgetState,ui->finalPageInputWidget);
    }
    AdapHmiMainWindow::retractExpandWidgets(10,90,91,301,Expand,ui->finalPageInputParametersVisionInputWidget);
    AdapHmiMainWindow::retractExpandWidgets(110,90,91,301,Expand,ui->finalPageInputParametersFilteringLevelWidget);
    AdapHmiMainWindow::retractExpandWidgets(210,90,91,301,Expand,ui->finalPageInputParametersNoMatchBehaviourWidget);
    AdapHmiMainWindow::retractExpandWidgets(100,110,800,350,Expand,ui->finalPageLutBlockWidget);
    if(!finalPageLookUpTableState->getWidgetState()){
        finalPageLookUpTableState->setWidgetState(ui->finalPageLutBlockWidget,true);
    }
    if(!finalPageOutput1WidgetState->getWidgetState()){
        AdapHmiMainWindow::retractExpandGivenWidget(0,8,151,101,finalPageOutput1WidgetState,ui->finalPageOutput1Widget);
    }
    if(!finalPageOutput2WidgetState->getWidgetState()){
        AdapHmiMainWindow::retractExpandGivenWidget(0,98,151,101,finalPageOutput2WidgetState,ui->finalPageOutput2Widget);
    }
    if(!finalPageOutput3WidgetState->getWidgetState()){
    AdapHmiMainWindow::retractExpandGivenWidget(0,188,151,101,finalPageOutput3WidgetState,ui->finalPageOutput3Widget);
    }
}
void AdapHmiMainWindow::showFinalPageInputWidget(){
    AdapHmiMainWindow::retractExpandGivenWidget(70,30,311,455,finalPageInputWidgetState,ui->finalPageInputWidget);
    AdapHmiMainWindow::retractExpandWidgets(10,90,91,301,Retract,ui->finalPageInputParametersVisionInputWidget);
    AdapHmiMainWindow::retractExpandWidgets(110,90,91,301,Retract,ui->finalPageInputParametersFilteringLevelWidget);
    AdapHmiMainWindow::retractExpandWidgets(210,90,91,301,Retract,ui->finalPageInputParametersNoMatchBehaviourWidget);
}
void AdapHmiMainWindow::saveFinalPageInputWidgetData(){
    AdapHmiMainWindow::retractExpandGivenWidget(70,30,311,455,finalPageInputWidgetState,ui->finalPageInputWidget);
    AdapHmiMainWindow::retractExpandWidgets(10,90,91,301,Expand,ui->finalPageInputParametersVisionInputWidget);
    AdapHmiMainWindow::retractExpandWidgets(110,90,91,301,Expand,ui->finalPageInputParametersFilteringLevelWidget);
    AdapHmiMainWindow::retractExpandWidgets(210,90,91,301,Expand,ui->finalPageInputParametersNoMatchBehaviourWidget);
    if(ui->finalPageInputParametersNoneFilteringLevelRadioButton->isChecked()){
        lutInputOutputMap.insert("INPUT_FILTER_LEVEL","NONE");
    }
    if(ui->finalPageInputParametersSmallFilteringLevelRadioButton->isChecked()){
        lutInputOutputMap.insert("INPUT_FILTER_LEVEL","LOW");
    }
    if(ui->finalPageInputParametersMediumFilteringLevelRadioButton->isChecked()){
        lutInputOutputMap.insert("INPUT_FILTER_LEVEL","MEDIUM");
    }
    if(ui->finalPageInputParametersHighFilteringLevelRadioButton->isChecked()){
        lutInputOutputMap.insert("INPUT_FILTER_LEVEL","HIGH");
    }
    if(ui->finalPageInputParametersMaintainOutputBehaviourRadioButton->isChecked()){
        lutInputOutputMap.insert("INPUT_IF_NOMATCH","MAINTAINOUTPUT");
    }
    if(ui->finalPageInputParametersMaintainOutputBehaviourRadioButton->isChecked()){
        lutInputOutputMap.insert("INPUT_IF_NOMATCH","RESETTONORMAL");
    }
}
void AdapHmiMainWindow::createFinalPageInputBlocks(int numberOfInputBlock, QMap<QString, QString> inputKeyValuePair){
    int keyCounter=0;
    switch(numberOfInputBlock){
    case 1:
         finalPageInputPushButton1 = new QPushButton(ui->finalPageInputGroupFrame);
         finalPageInputPushButton1->setGeometry(29,123,91,71);
         finalPageInputPushButton1->setStyleSheet("background-color: rgb(140, 140, 140);");
         finalPageInputPushButton1->setEnabled(false);
         finalPageInputPushButton1State->setWidgetState(ui->finalPageInputGroupFrame,true);
         foreach(QString key, inputKeyValuePair.keys()){
              QLabel *finalPageInputPushButton1Label1 = new QLabel(ui->finalPageInputGroupFrame);
              finalPageInputPushButton1Label1->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>GEO</span></p></body></html>");
              finalPageInputPushButton1Label1->setGeometry(43,123,65,21);
              QLabel *finalPageInputPushButton1Label2 = new QLabel(ui->finalPageInputGroupFrame);
              finalPageInputPushButton1Label2->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>"+inputKeyValuePair.value(key)+"</span></p></body></html>");
              finalPageInputPushButton1Label2->setGeometry(65,147,54,21);
         }
         QObject::connect(finalPageInputPushButton1,SIGNAL(clicked()),this,SLOT(showFinalPageInputWidget()));
         break;
    case 2:
         finalPageInputPushButton1 = new QPushButton(ui->finalPageInputGroupFrame);
         finalPageInputPushButton1->setGeometry(29,60,91,71);
         finalPageInputPushButton1->setStyleSheet("background-color: rgb(140, 140, 140);");
         finalPageInputPushButton1->setEnabled(false);
         finalPageInputPushButton1State->setWidgetState(ui->finalPageInputGroupFrame,true);
         finalPageInputPushButton2 = new QPushButton(ui->finalPageInputGroupFrame);
         finalPageInputPushButton2->setGeometry(29,186,91,71);
         finalPageInputPushButton2->setStyleSheet("background-color: rgb(140, 140, 140);");
         finalPageInputPushButton2->setEnabled(false);
         finalPageInputPushButton2State->setWidgetState(ui->finalPageInputGroupFrame,true);
         foreach(QString key, inputKeyValuePair.keys()){
              if(keyCounter == 0){
                  QLabel *finalPageInputPushButton1Label1 = new QLabel(ui->finalPageInputGroupFrame);
                  finalPageInputPushButton1Label1->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>GEO</span></p></body></html>");
                  finalPageInputPushButton1Label1->setGeometry(43,60,65,21);
                  QLabel *finalPageInputPushButton1Label2 = new QLabel(ui->finalPageInputGroupFrame);
                  finalPageInputPushButton1Label2->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>"+inputKeyValuePair.value(key)+"</span></p></body></html>");
                  finalPageInputPushButton1Label2->setGeometry(65,84,54,21);
              }
              if(keyCounter == 1){
                  QLabel *finalPageInputPushButton2Label1 = new QLabel(ui->finalPageInputGroupFrame);
                  finalPageInputPushButton2Label1->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>GEO</span></p></body></html>");
                  finalPageInputPushButton2Label1->setGeometry(43,186,65,21);
                  QLabel *finalPageInputPushButton2Label2 = new QLabel(ui->finalPageInputGroupFrame);
                  finalPageInputPushButton2Label2->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>"+inputKeyValuePair.value(key)+"</span></p></body></html>");
                  finalPageInputPushButton2Label2->setGeometry(65,210,54,21);
              }
              keyCounter++;
         }
         keyCounter=0;
         QObject::connect(finalPageInputPushButton1,SIGNAL(clicked()),this,SLOT(showFinalPageInputWidget()));
         QObject::connect(finalPageInputPushButton2,SIGNAL(clicked()),this,SLOT(showFinalPageInputWidget()));
         break;
    case 3:
         finalPageInputPushButton1 = new QPushButton(ui->finalPageInputGroupFrame);
         finalPageInputPushButton1->setGeometry(29,23,91,71);
         finalPageInputPushButton1->setStyleSheet("background-color: rgb(140, 140, 140);");
         finalPageInputPushButton1->setEnabled(false);
         finalPageInputPushButton1State->setWidgetState(ui->finalPageInputGroupFrame,true);
         finalPageInputPushButton2 = new QPushButton(ui->finalPageInputGroupFrame);
         finalPageInputPushButton2->setGeometry(29,113,91,71);
         finalPageInputPushButton2->setStyleSheet("background-color: rgb(140, 140, 140);");
         finalPageInputPushButton2->setEnabled(false);
         finalPageInputPushButton2State->setWidgetState(ui->finalPageInputGroupFrame,true);
         finalPageInputPushButton3 = new QPushButton(ui->finalPageInputGroupFrame);
         finalPageInputPushButton3->setGeometry(29,203,91,71);
         finalPageInputPushButton3->setStyleSheet("background-color: rgb(140, 140, 140);");
         finalPageInputPushButton3->setEnabled(false);
         finalPageInputPushButton3State->setWidgetState(ui->finalPageInputGroupFrame,true);
         foreach(QString key, inputKeyValuePair.keys()){
              if(keyCounter == 0){
                  QLabel *finalPageInputPushButton1Label1 = new QLabel(ui->finalPageInputGroupFrame);
                  finalPageInputPushButton1Label1->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>GEO</span></p></body></html>");
                  finalPageInputPushButton1Label1->setGeometry(43,23,65,21);
                  QLabel *finalPageInputPushButton1Label2 = new QLabel(ui->finalPageInputGroupFrame);
                  finalPageInputPushButton1Label2->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>"+inputKeyValuePair.value(key)+"</span></p></body></html>");
                  finalPageInputPushButton1Label2->setGeometry(65,47,54,21);
              }
              if(keyCounter == 1){
                  QLabel *finalPageInputPushButton2Label1 = new QLabel(ui->finalPageInputGroupFrame);
                  finalPageInputPushButton2Label1->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>GEO</span></p></body></html>");
                  finalPageInputPushButton2Label1->setGeometry(43,113,65,21);
                  QLabel *finalPageInputPushButton2Label2 = new QLabel(ui->finalPageInputGroupFrame);
                  finalPageInputPushButton2Label2->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>"+inputKeyValuePair.value(key)+"</span></p></body></html>");
                  finalPageInputPushButton2Label2->setGeometry(65,137,54,21);
              }
              if(keyCounter == 2){
                  QLabel *finalPageInputPushButton3Label1 = new QLabel(ui->finalPageInputGroupFrame);
                  finalPageInputPushButton3Label1->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>GEO</span></p></body></html>");
                  finalPageInputPushButton3Label1->setGeometry(43,203,65,21);
                  QLabel *finalPageInputPushButton3Label2 = new QLabel(ui->finalPageInputGroupFrame);
                  finalPageInputPushButton3Label2->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>"+inputKeyValuePair.value(key)+"</span></p></body></html>");
                  finalPageInputPushButton3Label2->setGeometry(65,227,54,21);
              }
              keyCounter++;
         }
         keyCounter=0;
         QObject::connect(finalPageInputPushButton1,SIGNAL(clicked()),this,SLOT(showFinalPageInputWidget()));
         QObject::connect(finalPageInputPushButton2,SIGNAL(clicked()),this,SLOT(showFinalPageInputWidget()));
         QObject::connect(finalPageInputPushButton3,SIGNAL(clicked()),this,SLOT(showFinalPageInputWidget()));
         break;
    default:
         keyCounter=0;
         break;
    }
}
void AdapHmiMainWindow::createFinalPageOutputBlocks(int numberOfOutputBlock, QMap<QString, QString> outputKeyValuePair){
    int keyCounter=0;
    switch(numberOfOutputBlock){
    case 1:
         finalPageOutputPushButton1 = new QPushButton(ui->finalPageOutputGroupFrame);
         finalPageOutputPushButton1->setGeometry(29,123,91,71);
         finalPageOutputPushButton1->setStyleSheet("background-color: rgb(140, 140, 140);");
         finalPageOutputPushButton1->setEnabled(false);
         finalPageOutputPushButton1State->setWidgetState(ui->finalPageOutputGroupFrame,true);
         foreach(QString key, outputKeyValuePair.keys()){
              QLabel *finalPageOutputPushButton1Label1 = new QLabel(ui->finalPageOutputGroupFrame);
              finalPageOutputPushButton1Label1->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>"+key+"</span></p></body></html>");
              finalPageOutputPushButton1Label1->setGeometry(43,123,65,21);
              QLabel *finalPageOutputPushButton1Label2 = new QLabel(ui->finalPageOutputGroupFrame);
              finalPageOutputPushButton1Label2->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>"+outputKeyValuePair.value(key)+"</span></p></body></html>");
              finalPageOutputPushButton1Label2->setGeometry(43,171,65,21);
              AdapHmiMainWindow::configureFinalPageOutputBlocks(key,keyCounter);
         }
         QObject::connect(finalPageOutputPushButton1,SIGNAL(clicked()),this,SLOT(showFinalPageOutput1Widget()));
         QObject::connect(ui->finalPageOutput1SaveButton,SIGNAL(clicked()),this,SLOT(saveFinalPageOutput1Widget()));
         break;
    case 2:
         finalPageOutputPushButton1 = new QPushButton(ui->finalPageOutputGroupFrame);
         finalPageOutputPushButton1->setGeometry(29,60,91,71);
         finalPageOutputPushButton1->setStyleSheet("background-color: rgb(140, 140, 140);");
         finalPageOutputPushButton1->setEnabled(false);
         finalPageOutputPushButton1State->setWidgetState(ui->finalPageOutputGroupFrame,true);
         finalPageOutputPushButton2 = new QPushButton(ui->finalPageOutputGroupFrame);
         finalPageOutputPushButton2->setGeometry(29,186,91,71);
         finalPageOutputPushButton2->setStyleSheet("background-color: rgb(140, 140, 140);");
         finalPageOutputPushButton2->setEnabled(false);
         finalPageOutputPushButton2State->setWidgetState(ui->finalPageOutputGroupFrame,true);
         foreach(QString key, outputKeyValuePair.keys()){
              if(keyCounter == 0){
                  QLabel *finalPageOutputPushButton1Label1 = new QLabel(ui->finalPageOutputGroupFrame);
                  finalPageOutputPushButton1Label1->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>"+key+"</span></p></body></html>");
                  finalPageOutputPushButton1Label1->setGeometry(43,60,65,21);
                  QLabel *finalPageOutputPushButton1Label2 = new QLabel(ui->finalPageOutputGroupFrame);
                  finalPageOutputPushButton1Label2->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>"+outputKeyValuePair.value(key)+"</span></p></body></html>");
                  finalPageOutputPushButton1Label2->setGeometry(43,108,65,21);
                  AdapHmiMainWindow::configureFinalPageOutputBlocks(key,keyCounter);
              }
              if(keyCounter == 1){
                  QLabel *finalPageOutputPushButton2Label1 = new QLabel(ui->finalPageOutputGroupFrame);
                  finalPageOutputPushButton2Label1->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>"+key+"</span></p></body></html>");
                  finalPageOutputPushButton2Label1->setGeometry(43,186,65,21);
                  QLabel *finalPageOutputPushButton2Label2 = new QLabel(ui->finalPageOutputGroupFrame);
                  finalPageOutputPushButton2Label2->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>"+outputKeyValuePair.value(key)+"</span></p></body></html>");
                  finalPageOutputPushButton2Label2->setGeometry(43,234,65,21);
                  AdapHmiMainWindow::configureFinalPageOutputBlocks(key,keyCounter);
              }
              keyCounter++;
         }
         keyCounter=0;
         QObject::connect(finalPageOutputPushButton1,SIGNAL(clicked()),this,SLOT(showFinalPageOutput1Widget()));
         QObject::connect(finalPageOutputPushButton2,SIGNAL(clicked()),this,SLOT(showFinalPageOutput2Widget()));
         QObject::connect(ui->finalPageOutput1SaveButton,SIGNAL(clicked()),this,SLOT(saveFinalPageOutput1Widget()));
         QObject::connect(ui->finalPageOutput2SaveButton,SIGNAL(clicked()),this,SLOT(saveFinalPageOutput2Widget()));
         break;
    case 3:
         finalPageOutputPushButton1 = new QPushButton(ui->finalPageOutputGroupFrame);
         finalPageOutputPushButton1->setGeometry(29,23,91,71);
         finalPageOutputPushButton1->setStyleSheet("background-color: rgb(140, 140, 140);");
         finalPageOutputPushButton1->setEnabled(false);
         finalPageOutputPushButton1State->setWidgetState(ui->finalPageOutputGroupFrame,true);
         finalPageOutputPushButton2 = new QPushButton(ui->finalPageOutputGroupFrame);
         finalPageOutputPushButton2->setGeometry(29,113,91,71);
         finalPageOutputPushButton2->setStyleSheet("background-color: rgb(140, 140, 140);");
         finalPageOutputPushButton2->setEnabled(false);
         finalPageOutputPushButton2State->setWidgetState(ui->finalPageOutputGroupFrame,true);
         finalPageOutputPushButton3 = new QPushButton(ui->finalPageOutputGroupFrame);
         finalPageOutputPushButton3->setGeometry(29,203,91,71);
         finalPageOutputPushButton3->setStyleSheet("background-color: rgb(140, 140, 140);");
         finalPageOutputPushButton3->setEnabled(false);
         finalPageOutputPushButton3State->setWidgetState(ui->finalPageOutputGroupFrame,true);
         foreach(QString key, outputKeyValuePair.keys()){
              if(keyCounter == 0){
                  QLabel *finalPageOutputPushButton1Label1 = new QLabel(ui->finalPageOutputGroupFrame);
                  finalPageOutputPushButton1Label1->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>"+key+"</span></p></body></html>");
                  finalPageOutputPushButton1Label1->setGeometry(43,23,65,21);
                  QLabel *finalPageOutputPushButton1Label2 = new QLabel(ui->finalPageOutputGroupFrame);
                  finalPageOutputPushButton1Label2->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>"+outputKeyValuePair.value(key)+"</span></p></body></html>");
                  finalPageOutputPushButton1Label2->setGeometry(43,71,65,21);
                  AdapHmiMainWindow::configureFinalPageOutputBlocks(key,keyCounter);
              }
              if(keyCounter == 1){
                  QLabel *finalPageOutputPushButton2Label1 = new QLabel(ui->finalPageOutputGroupFrame);
                  finalPageOutputPushButton2Label1->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>"+key+"</span></p></body></html>");
                  finalPageOutputPushButton2Label1->setGeometry(43,113,65,21);
                  QLabel *finalPageOutputPushButton2Label2 = new QLabel(ui->finalPageOutputGroupFrame);
                  finalPageOutputPushButton2Label2->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>"+outputKeyValuePair.value(key)+"</span></p></body></html>");
                  finalPageOutputPushButton2Label2->setGeometry(43,161,65,21);
                  AdapHmiMainWindow::configureFinalPageOutputBlocks(key,keyCounter);
              }
              if(keyCounter == 2){
                  QLabel *finalPageOutputPushButton3Label1 = new QLabel(ui->finalPageOutputGroupFrame);
                  finalPageOutputPushButton3Label1->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>"+key+"</span></p></body></html>");
                  finalPageOutputPushButton3Label1->setGeometry(43,203,65,21);
                  QLabel *finalPageOutputPushButton3Label2 = new QLabel(ui->finalPageOutputGroupFrame);
                  finalPageOutputPushButton3Label2->setText("<html><head/><body><p align='center'><span style=' font-size:12pt; font-weight:600;'>"+outputKeyValuePair.value(key)+"</span></p></body></html>");
                  finalPageOutputPushButton3Label2->setGeometry(43,251,65,21);
                  AdapHmiMainWindow::configureFinalPageOutputBlocks(key,keyCounter);
              }
              keyCounter++;
         }
         keyCounter=0;
         QObject::connect(finalPageOutputPushButton1,SIGNAL(clicked()),this,SLOT(showFinalPageOutput1Widget()));
         QObject::connect(finalPageOutputPushButton2,SIGNAL(clicked()),this,SLOT(showFinalPageOutput2Widget()));
         QObject::connect(finalPageOutputPushButton3,SIGNAL(clicked()),this,SLOT(showFinalPageOutput3Widget()));
         QObject::connect(ui->finalPageOutput1SaveButton,SIGNAL(clicked()),this,SLOT(saveFinalPageOutput1Widget()));
         QObject::connect(ui->finalPageOutput2SaveButton,SIGNAL(clicked()),this,SLOT(saveFinalPageOutput2Widget()));
         QObject::connect(ui->finalPageOutput3SaveButton,SIGNAL(clicked()),this,SLOT(saveFinalPageOutput3Widget()));
         break;
    default:
         keyCounter=0;
         break;
    }
}
void AdapHmiMainWindow::showFinalPageOutput1Widget(){
    ui->finalPageOutput1Widget->raise();
    if(finalPageOutputPushButton1State->getWidgetState() && finalPageOutputPushButton2State->getWidgetState() && finalPageOutputPushButton3State->getWidgetState()){
        AdapHmiMainWindow::retractExpandGivenWidget(0,8,151,101,finalPageOutput1WidgetState,ui->finalPageOutput1Widget);
        if(!finalPageOutput2WidgetState->getWidgetState()){
            AdapHmiMainWindow::retractExpandGivenWidget(0,98,151,101,finalPageOutput2WidgetState,ui->finalPageOutput2Widget);
        }
        if(!finalPageOutput3WidgetState->getWidgetState()){
            AdapHmiMainWindow::retractExpandGivenWidget(0,188,151,101,finalPageOutput3WidgetState,ui->finalPageOutput3Widget);
        }
    } else {
        if(finalPageOutputPushButton1State->getWidgetState() && finalPageOutputPushButton2State->getWidgetState()){
            AdapHmiMainWindow::retractExpandGivenWidget(0,15,151,101,finalPageOutput1WidgetState,ui->finalPageOutput1Widget);
            if(!finalPageOutput2WidgetState->getWidgetState()){
                AdapHmiMainWindow::retractExpandGivenWidget(0,171,151,101,finalPageOutput2WidgetState,ui->finalPageOutput2Widget);
            }
        } else {
            if(finalPageOutputPushButton1State->getWidgetState()){
                AdapHmiMainWindow::retractExpandGivenWidget(0,108,151,101,finalPageOutput1WidgetState,ui->finalPageOutput1Widget);
            }
        }
    }
}
void AdapHmiMainWindow::showFinalPageOutput2Widget(){
    ui->finalPageOutput2Widget->raise();
    if(finalPageOutputPushButton1State->getWidgetState() && finalPageOutputPushButton2State->getWidgetState() && finalPageOutputPushButton3State->getWidgetState()){
        AdapHmiMainWindow::retractExpandGivenWidget(0,98,151,101,finalPageOutput2WidgetState,ui->finalPageOutput2Widget);
        if(!finalPageOutput1WidgetState->getWidgetState()){
            AdapHmiMainWindow::retractExpandGivenWidget(0,8,151,101,finalPageOutput1WidgetState,ui->finalPageOutput1Widget);
        }
        if(!finalPageOutput3WidgetState->getWidgetState()){
            AdapHmiMainWindow::retractExpandGivenWidget(0,188,151,101,finalPageOutput3WidgetState,ui->finalPageOutput3Widget);
        }
    } else {
        if(finalPageOutputPushButton1State->getWidgetState() && finalPageOutputPushButton2State->getWidgetState()){
            AdapHmiMainWindow::retractExpandGivenWidget(0,171,151,101,finalPageOutput2WidgetState,ui->finalPageOutput2Widget);
            if(!finalPageOutput1WidgetState->getWidgetState()){
                AdapHmiMainWindow::retractExpandGivenWidget(0,15,151,101,finalPageOutput1WidgetState,ui->finalPageOutput1Widget);
            }
        }
    }
}
void AdapHmiMainWindow::showFinalPageOutput3Widget(){
    ui->finalPageOutput3Widget->raise();
    if(finalPageOutputPushButton1State->getWidgetState() && finalPageOutputPushButton2State->getWidgetState() && finalPageOutputPushButton3State->getWidgetState()){
        AdapHmiMainWindow::retractExpandGivenWidget(0,188,151,101,finalPageOutput3WidgetState,ui->finalPageOutput3Widget);
        if(!finalPageOutput1WidgetState->getWidgetState()){
            AdapHmiMainWindow::retractExpandGivenWidget(0,8,151,101,finalPageOutput1WidgetState,ui->finalPageOutput1Widget);
        }
        if(!finalPageOutput2WidgetState->getWidgetState()){
            AdapHmiMainWindow::retractExpandGivenWidget(0,98,151,101,finalPageOutput2WidgetState,ui->finalPageOutput2Widget);
        }
    }
}
void AdapHmiMainWindow::saveFinalPageOutput1Widget(){
    AdapHmiMainWindow::retractExpandWidgets(0,8,151,101,Expand,ui->finalPageOutput1Widget);
    if(identifyOutputMap.contains(0)){
        if(identifyOutputMap.value(0) == "I"){
            lutInputOutputMap.insert("OUTPUT_CURRENT_FILTER_LEVEL",QString::number(ui->finalPageOutput1FilterComboBox->currentIndex()));
            lutInputOutputMap.insert("OUTPUT_CURRENT_IF_NOMATCH",QString::number(ui->finalPageOutput1NoMatchComboBox->currentIndex()));
        }
        if(identifyOutputMap.value(0) == "V"){
            lutInputOutputMap.insert("OUTPUT_VOLTAGE_FILTER_LEVEL",QString::number(ui->finalPageOutput1FilterComboBox->currentIndex()));
            lutInputOutputMap.insert("OUTPUT_VOLTAGE_IF_NOMATCH",QString::number(ui->finalPageOutput1NoMatchComboBox->currentIndex()));
        }
        if(identifyOutputMap.value(0) == "TRAVEL"){
            lutInputOutputMap.insert("OUTPUT_TRAVELSPEED_FILTER_LEVEL",QString::number(ui->finalPageOutput1FilterComboBox->currentIndex()));
            lutInputOutputMap.insert("OUTPUT_TRAVELSPEED_IF_NOMATCH",QString::number(ui->finalPageOutput1NoMatchComboBox->currentIndex()));
        }
        if(identifyOutputMap.value(0) == "WIRE"){
            lutInputOutputMap.insert("OUTPUT_WIREFEED_FILTER_LEVEL",QString::number(ui->finalPageOutput1FilterComboBox->currentIndex()));
            lutInputOutputMap.insert("OUTPUT_WIREFEED_IF_NOMATCH",QString::number(ui->finalPageOutput1NoMatchComboBox->currentIndex()));
        }
    }
}
void AdapHmiMainWindow::saveFinalPageOutput2Widget(){
    AdapHmiMainWindow::retractExpandWidgets(0,98,151,101,Expand,ui->finalPageOutput2Widget);
    if(identifyOutputMap.contains(1)){
        if(identifyOutputMap.value(1) == "I"){
            lutInputOutputMap.insert("OUTPUT_CURRENT_FILTER_LEVEL",QString::number(ui->finalPageOutput2FilterComboBox->currentIndex()));
            lutInputOutputMap.insert("OUTPUT_CURRENT_IF_NOMATCH",QString::number(ui->finalPageOutput2NoMatchComboBox->currentIndex()));
        }
        if(identifyOutputMap.value(1) == "V"){
            lutInputOutputMap.insert("OUTPUT_VOLTAGE_FILTER_LEVEL",QString::number(ui->finalPageOutput2FilterComboBox->currentIndex()));
            lutInputOutputMap.insert("OUTPUT_VOLTAGE_IF_NOMATCH",QString::number(ui->finalPageOutput2NoMatchComboBox->currentIndex()));
        }
        if(identifyOutputMap.value(1) == "TRAVEL"){
            lutInputOutputMap.insert("OUTPUT_TRAVELSPEED_FILTER_LEVEL",QString::number(ui->finalPageOutput2FilterComboBox->currentIndex()));
            lutInputOutputMap.insert("OUTPUT_TRAVELSPEED_IF_NOMATCH",QString::number(ui->finalPageOutput2NoMatchComboBox->currentIndex()));
        }
        if(identifyOutputMap.value(1) == "WIRE"){
            lutInputOutputMap.insert("OUTPUT_WIREFEED_FILTER_LEVEL",QString::number(ui->finalPageOutput2FilterComboBox->currentIndex()));
            lutInputOutputMap.insert("OUTPUT_WIREFEED_IF_NOMATCH",QString::number(ui->finalPageOutput2NoMatchComboBox->currentIndex()));
        }
    }
}
void AdapHmiMainWindow::saveFinalPageOutput3Widget(){
    AdapHmiMainWindow::retractExpandWidgets(0,188,151,101,Expand,ui->finalPageOutput3Widget);
    if(identifyOutputMap.contains(2)){
        if(identifyOutputMap.value(2) == "I"){
            lutInputOutputMap.insert("OUTPUT_CURRENT_FILTER_LEVEL",QString::number(ui->finalPageOutput3FilterComboBox->currentIndex()));
            lutInputOutputMap.insert("OUTPUT_CURRENT_IF_NOMATCH",QString::number(ui->finalPageOutput3NoMatchComboBox->currentIndex()));
        }
        if(identifyOutputMap.value(2) == "V"){
            lutInputOutputMap.insert("OUTPUT_VOLTAGE_FILTER_LEVEL",QString::number(ui->finalPageOutput3FilterComboBox->currentIndex()));
            lutInputOutputMap.insert("OUTPUT_VOLTAGE_IF_NOMATCH",QString::number(ui->finalPageOutput3NoMatchComboBox->currentIndex()));
        }
        if(identifyOutputMap.value(2) == "TRAVEL"){
            lutInputOutputMap.insert("OUTPUT_TRAVELSPEED_FILTER_LEVEL",QString::number(ui->finalPageOutput3FilterComboBox->currentIndex()));
            lutInputOutputMap.insert("OUTPUT_TRAVELSPEED_IF_NOMATCH",QString::number(ui->finalPageOutput3NoMatchComboBox->currentIndex()));
        }
        if(identifyOutputMap.value(2) == "WIRE"){
            lutInputOutputMap.insert("OUTPUT_WIREFEED_FILTER_LEVEL",QString::number(ui->finalPageOutput3FilterComboBox->currentIndex()));
            lutInputOutputMap.insert("OUTPUT_WIREFEED_IF_NOMATCH",QString::number(ui->finalPageOutput3NoMatchComboBox->currentIndex()));
        }
    }
}

void AdapHmiMainWindow::configureFinalPageOutputBlocks(QString key, int keyCounter){
    if(keyCounter == 0){
        if (key == "I"){
            if(lutInputOutputMap.contains("OUTPUT_CURRENT_FILTER_LEVEL")){
                ui->finalPageOutput1FilterComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_CURRENT_FILTER_LEVEL").toInt());
            }
            if(lutInputOutputMap.contains("OUTPUT_CURRENT_IF_NOMATCH")){
                ui->finalPageOutput1NoMatchComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_CURRENT_IF_NOMATCH").toInt());
            }
        }
        if (key == "V"){
            if(lutInputOutputMap.contains("OUTPUT_VOLTAGE_FILTER_LEVEL")){
                ui->finalPageOutput1FilterComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_VOLTAGE_FILTER_LEVEL").toInt());
            }
            if(lutInputOutputMap.contains("OUTPUT_VOLTAGE_IF_NOMATCH")){
                ui->finalPageOutput1NoMatchComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_VOLTAGE_IF_NOMATCH").toInt());
            }
        }
        if (key == "WIRE"){
            if(lutInputOutputMap.contains("OUTPUT_WIREFEED_FILTER_LEVEL")){
                ui->finalPageOutput1FilterComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_WIREFEED_FILTER_LEVEL").toInt());
            }
            if(lutInputOutputMap.contains("OUTPUT_WIREFEED_IF_NOMATCH")){
                ui->finalPageOutput1NoMatchComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_WIREFEED_IF_NOMATCH").toInt());
            }
        }
        if (key == "TRAVEL"){
            if(lutInputOutputMap.contains("OUTPUT_TRAVELSPEED_FILTER_LEVEL")){
                ui->finalPageOutput1FilterComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_TRAVELSPEED_FILTER_LEVEL").toInt());
            }
            if(lutInputOutputMap.contains("OUTPUT_TRAVELSPEED_IF_NOMATCH")){
                ui->finalPageOutput1NoMatchComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_TRAVELSPEED_IF_NOMATCH").toInt());
            }
        }
    }
    if(keyCounter == 1){
        if (key == "I"){
            if(lutInputOutputMap.contains("OUTPUT_CURRENT_FILTER_LEVEL")){
                ui->finalPageOutput2FilterComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_CURRENT_FILTER_LEVEL").toInt());
            }
            if(lutInputOutputMap.contains("OUTPUT_CURRENT_IF_NOMATCH")){
                ui->finalPageOutput2NoMatchComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_CURRENT_IF_NOMATCH").toInt());
            }
        }
        if (key == "V"){
            if(lutInputOutputMap.contains("OUTPUT_VOLTAGE_FILTER_LEVEL")){
                ui->finalPageOutput2FilterComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_VOLTAGE_FILTER_LEVEL").toInt());
            }
            if(lutInputOutputMap.contains("OUTPUT_VOLTAGE_IF_NOMATCH")){
                ui->finalPageOutput2NoMatchComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_VOLTAGE_IF_NOMATCH").toInt());
            }
        }
        if (key == "WIRE"){
            if(lutInputOutputMap.contains("OUTPUT_WIREFEED_FILTER_LEVEL")){
                ui->finalPageOutput2FilterComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_WIREFEED_FILTER_LEVEL").toInt());
            }
            if(lutInputOutputMap.contains("OUTPUT_WIREFEED_IF_NOMATCH")){
                ui->finalPageOutput2NoMatchComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_WIREFEED_IF_NOMATCH").toInt());
            }
        }
        if (key == "TRAVEL"){
            if(lutInputOutputMap.contains("OUTPUT_TRAVELSPEED_FILTER_LEVEL")){
                ui->finalPageOutput2FilterComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_TRAVELSPEED_FILTER_LEVEL").toInt());
            }
            if(lutInputOutputMap.contains("OUTPUT_TRAVELSPEED_IF_NOMATCH")){
                ui->finalPageOutput2NoMatchComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_TRAVELSPEED_IF_NOMATCH").toInt());
            }
        }
    }
    if(keyCounter == 2){
        if (key == "I"){
            if(lutInputOutputMap.contains("OUTPUT_CURRENT_FILTER_LEVEL")){
                ui->finalPageOutput3FilterComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_CURRENT_FILTER_LEVEL").toInt());
            }
            if(lutInputOutputMap.contains("OUTPUT_CURRENT_IF_NOMATCH")){
                ui->finalPageOutput3NoMatchComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_CURRENT_IF_NOMATCH").toInt());
            }
        }
        if (key == "V"){
            if(lutInputOutputMap.contains("OUTPUT_VOLTAGE_FILTER_LEVEL")){
                ui->finalPageOutput3FilterComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_VOLTAGE_FILTER_LEVEL").toInt());
            }
            if(lutInputOutputMap.contains("OUTPUT_VOLTAGE_IF_NOMATCH")){
                ui->finalPageOutput3NoMatchComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_VOLTAGE_IF_NOMATCH").toInt());
            }
        }
        if (key == "WIRE"){
            if(lutInputOutputMap.contains("OUTPUT_WIREFEED_FILTER_LEVEL")){
                ui->finalPageOutput3FilterComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_WIREFEED_FILTER_LEVEL").toInt());
            }
            if(lutInputOutputMap.contains("OUTPUT_WIREFEED_IF_NOMATCH")){
                ui->finalPageOutput3NoMatchComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_WIREFEED_IF_NOMATCH").toInt());
            }
        }
        if (key == "TRAVEL"){
            if(lutInputOutputMap.contains("OUTPUT_TRAVELSPEED_FILTER_LEVEL")){
                ui->finalPageOutput3FilterComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_TRAVELSPEED_FILTER_LEVEL").toInt());
            }
            if(lutInputOutputMap.contains("OUTPUT_TRAVELSPEED_IF_NOMATCH")){
                ui->finalPageOutput3NoMatchComboBox->setCurrentIndex(lutInputOutputMap.value("OUTPUT_TRAVELSPEED_IF_NOMATCH").toInt());
            }
        }
    }
    identifyOutputMap.insert(keyCounter,key);
}
bool AdapHmiMainWindow::adapArcWeldingParametersValidation(){
    if(ui->arcWeldingWireFeedLineEdit->text().isEmpty() || ui->arcWeldingCurrentLineEdit->text().isEmpty() || ui->arcWeldingVoltageLineEdit->text().isEmpty() || ui->arcWeldingTravelSpeedLineEdit->text().isEmpty() || (!ui->arcWeldingWeavingReqYesRadioButton->isChecked() && !ui->arcWeldingWeavingReqNoRadioButton->isChecked()) || (ui->arcWeldingWeavingReqYesRadioButton->isChecked() && (ui->arcWeldingFrequencyLineEdit->text().isEmpty() || ui->arcWeldingAmplitudeLineEdit->text().isEmpty()))){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet("background-color: rgb(203, 203, 203); text-align: centre;");
        msgBox.exec();
        return false;
    }
    return true;
}
void AdapHmiMainWindow::showFinalPagelutWidget(){
    AdapHmiMainWindow::retractExpandGivenWidget(100,110,800,350,finalPageLookUpTableState,ui->finalPageLutBlockWidget);
    AdapHmiMainWindow::prepareFinalPageLutBlockFromMap();
}
void AdapHmiMainWindow::saveFinalPagelutWidgetData(){
    AdapHmiMainWindow::retractExpandGivenWidget(100,110,800,350,finalPageLookUpTableState,ui->finalPageLutBlockWidget);
    AdapHmiMainWindow::saveFinalPageLutBlockToMap();
}
void AdapHmiMainWindow::prepareFinalPageLutBlockFromMap(){
    int columnCount=0;
    foreach(QString key, lutInputOutputMap.keys()){
        if(key.contains("IP_") || key.contains("OP_")){
            columnCount++;
        }
    }
    QStandardItemModel *finalLutModel = new QStandardItemModel(3,columnCount,this);
    columnCount=0;
    foreach(QString key, lutInputOutputMap.keys()){
        if(key.contains("IP_") || key.contains("OP_")){
            if(lutInputOutputMap.value(key) == "YES"){
                finalLutModel->setHorizontalHeaderItem(columnCount, new QStandardItem(key.mid(3,key.length()-3)));
            }
            columnCount++;
        }
    }
    finalLutModel->setHorizontalHeaderItem(columnCount, new QStandardItem(QIcon(":adapHMIResources/images/Ico_Delete.png"),""));
    foreach( adaphmisavelookuptable* key, lutMap.keys()){
           finalLutModel->setItem(key->getRow(),key->getCol(),new QStandardItem(lutMap.value(key)));
           finalLutModel->setItem(key->getRow(),columnCount, new QStandardItem(QIcon(":adapHMIResources/images/Ico_Delete.png"),""));
    }
    ui->finalPageLutView->setModel(lutModel);
    ui->finalPageLutView->horizontalHeader()->setVisible(true);
    ui->finalPageLutView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->finalPageLutView->horizontalHeader()->setClickable(false);
    ui->finalPageLutView->verticalHeader()->setVisible(false);
    ui->finalPageLutView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    QObject::connect(ui->finalPageLutView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),SLOT(insertFinalPageLookUpTableViewOnSelectionChange(const QItemSelection &, const QItemSelection &)));
}
void AdapHmiMainWindow::insertFinalPageLookUpTableViewOnSelectionChange(const QItemSelection &, const QItemSelection &){
    QModelIndexList selectedRowInLutTableIndexes = ui->finalPageLutView->selectionModel()->selection().indexes();
    for (int i = 0; i < selectedRowInLutTableIndexes.count(); ++i){
        QModelIndex lutIndex = selectedRowInLutTableIndexes.at(i);
        QAbstractItemModel *previousLutModel = ui->finalPageLutView->model();
        if(lutIndex.row() == previousLutModel->rowCount()-1 && lutIndex.column() == 0){
            previousLutModel->insertRow(previousLutModel->rowCount());
            previousLutModel->setData(previousLutModel->index(lutIndex.row()+1,previousLutModel->columnCount()-1,QModelIndex()),QIcon(":adapHMIResources/images/Ico_Delete.png"),Qt::DecorationRole);
        }
        if(lutIndex.column() == previousLutModel->columnCount()-1 && previousLutModel->rowCount() > 1){
            previousLutModel->removeRows(lutIndex.row(),1);
        }
    }
}
void AdapHmiMainWindow::saveFinalPageLutBlockToMap(){
    lutMap.clear();
    QAbstractItemModel *tempLutModel = ui->finalPageLutView->model();
    int row = tempLutModel->rowCount();
    int col = tempLutModel->columnCount();
    for (int i = 0; i < row ; i++){
        for (int j = 0; j < col - 1; j++){
            if(!tempLutModel->data(tempLutModel->index(i,j), Qt::DisplayRole).toString().isEmpty()){
                lutMap.insert(new adaphmisavelookuptable(i,j),tempLutModel->data(tempLutModel->index(i,j), Qt::DisplayRole).toString());
            }
        }
    }
}
void AdapHmiMainWindow::callTinyXMLApiToGenerateAdapXML(){
    QString saveGenerateOutPutXML = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),tr("XML File (*.xml)"));
    adaphmixmlcreator * xmlCreatorObject = new adaphmixmlcreator(saveGenerateOutPutXML);
    xmlCreatorObject->createadaphmiXMLElements(lutInputOutputMap,lutMap);
    ui->finalPageCloseScreenButton->setEnabled(true);
}

void AdapHmiMainWindow::enableDisableOutPutCheckBoxes(int val){
    if(val == 1){
        outputCount++;
        if(outputCount == 3){
            if(!ui->arcWeldingOutputVoltageCheckBox->isChecked()){
                ui->arcWeldingOutputVoltageCheckBox->setEnabled(false);
            }
            if(!ui->arcWeldingOutputCurrentCheckBox->isChecked()){
                ui->arcWeldingOutputCurrentCheckBox->setEnabled(false);
            }
            if(!ui->arcWeldingOutputTravelSpeedCheckBox->isChecked()){
                ui->arcWeldingOutputTravelSpeedCheckBox->setEnabled(false);
            }
            if(!ui->arcWeldingOutputWireFeedCheckBox->isChecked()){
                ui->arcWeldingOutputWireFeedCheckBox->setEnabled(false);
            }
        }
    }
    if(val == 0){
        outputCount--;
        if(!ui->arcWeldingOutputVoltageCheckBox->isEnabled()){
            ui->arcWeldingOutputVoltageCheckBox->setEnabled(true);
        }
        if(!ui->arcWeldingOutputCurrentCheckBox->isEnabled()){
            ui->arcWeldingOutputCurrentCheckBox->setEnabled(true);
        }
        if(!ui->arcWeldingOutputTravelSpeedCheckBox->isEnabled()){
            ui->arcWeldingOutputTravelSpeedCheckBox->setEnabled(true);
        }
        if(!ui->arcWeldingOutputWireFeedCheckBox->isEnabled()){
            ui->arcWeldingOutputWireFeedCheckBox->setEnabled(true);
        }
    }
}

bool AdapHmiMainWindow::inputParametersValidation(){
    if(!(ui->arcWeldingInputParametersAreaCheckBox->isChecked() || ui->arcWeldingInputParametersGapCheckBox->isChecked() || ui->arcWeldingInputParametersMismatchCheckBox->isChecked()) || !(ui->arcWeldingInputParametersNoneFilteringLevelRadioButton->isChecked() || ui->arcWeldingInputParametersSmallFilteringLevelRadioButton->isChecked() || ui->arcWeldingInputParametersMediumFilteringLevelRadioButton->isChecked() || ui->arcWeldingInputParametersHighFilteringLevelRadioButton->isChecked()) || !(ui->arcWeldingInputParametersMaintainOutputBehaviourRadioButton->isChecked() || ui->arcWeldingInputParametersResetToNormalBehaviourRadioButton->isChecked())){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet("background-color: rgb(203, 203, 203); text-align: centre;");
        msgBox.exec();
        return false;
    }
    return true;
}
bool AdapHmiMainWindow::outputParametersValidation(){
    if(!(ui->arcWeldingOutputWireFeedCheckBox->isChecked() || ui->arcWeldingOutputCurrentCheckBox->isChecked() || ui->arcWeldingOutputVoltageCheckBox->isChecked() || ui->arcWeldingOutputTravelSpeedCheckBox->isChecked())){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet("background-color: rgb(203, 203, 203); text-align: centre;");
        msgBox.exec();
        return false;
    }
    return true;
}
