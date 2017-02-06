#ifndef ADAPHMIMAINWINDOW_H
#define ADAPHMIMAINWINDOW_H

#include <QMainWindow>
#include "adaphmiwidgetstate.h"
#include "adaphmisaveload.h"
#include "adaphmisavelookuptable.h"
#include <QMap>
#include <QStandardItemModel>
#include <QItemSelection>
#include <QPushButton>

namespace Ui {
class AdapHmiMainWindow;
}

class AdapHmiMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdapHmiMainWindow(QWidget *parent = 0);
    ~AdapHmiMainWindow();
    enum RequestTypes
    {
         Expand,
         Retract
    };
    enum OutputInitials
    {
         I,
         V,
         TRAVEL,
         WIRE
    };
    void createSlotsForAdapHmiStackedWidget();
    void createSlotsForInputOutputParameterStackedWidget();
    void createObjectConnects();
    void initializeAllWidgets();
    void retractAllWidgets();
    void retractExpandWidgets(int X,int Y,int W,int H, RequestTypes type,QObject* qInputWidget);
    void retractGivenWidget(int X, int Y, int W, int H,adaphmiwidgetstate *givenWidgetState,QWidget *givenWidget=0);
    void retractExpandGivenWidget(int X, int Y, int W, int H,adaphmiwidgetstate *givenWidgetState,QWidget *givenWidget=0);
    void createlutTableBasedOnInputOutputParameters();
    void loadlutInputOutputMapFromSettings();
    void loadlookuptableMapFromSettings();
    void createBlockDiagramFromQMap();
    void createFinalPageInputBlocks(int numberOfInputBlock, QMap<QString, QString> inputKeyValuePair);
    void createFinalPageOutputBlocks(int numberOfOutputBlock, QMap<QString,QString> outputKeyValuePair);
    void configureFinalPageOutputBlocks(QString key, int keyCounter);
    bool adapArcWeldingParametersValidation();
    void prepareFinalPageLutBlockFromMap();
    void saveFinalPageLutBlockToMap();
    void enableDisableOutPutCheckBoxes(int val);
    bool inputParametersValidation();
    bool outputParametersValidation();

private slots:
    void sendToNextAdapHmiStackedWidget(int index=0);
    void sendToNextInputOutputParameterStackedWidget(int index=0);
    void showReviewBlockDiagramWeldingParametersWidgetOnClicked();
    void showArcWeldingParametersWidgetOnClick();
    void showArcWeldingAdditionalParameterWidgetOnClick();
    void showReviewBlockDiagramSettingsWidgetOnClick();
    void showAdapParametersSettingsWidgetOnClick();
    void showArcWeldingInputParametersVisionInputWidgetOnClick();
    void showArcWeldingInputParametersFilteringLevelWidgetOnClick();
    void showArcWeldingInputParametersNoMatchBehaviourWidgetOnClick();
    void showArcWeldingOutputParametersBodyWidgetOnClick();
    void showArcWeldingOutputAdditionalParameterWidgetOnSelection(bool checked);
    void performActionArcWeldingWeavingReqNoRadioButtonOnClick(bool checked);
    void showArcWeldingLookUpTableViewOnClick();
    void storeArcWeldingWireFeedLineEdit();
    void storeArcWeldingCurrentLineEdit();
    void storeArcWeldingVoltageLineEdit();
    void storeArcWeldingTravelSpeedLineEdit();
    void storeArcWeldingAmplitudeLineEdit();
    void storeArcWeldingFrequencyLineEdit();
    void storeArcWeldingInputParametersAreaCheckBoxOnSelection(bool checked);
    void storeArcWeldingInputParametersGapCheckBoxOnSelection(bool checked);
    void storeArcWeldingInputParametersMismatchCheckBoxOnSelection(bool checked);
    void storeArcWeldingInputParametersHighFilteringLevelRadioButtonOnSelection(bool checked);
    void storeArcWeldingInputParametersMediumFilteringLevelRadioButtonOnSelection(bool checked);
    void storeArcWeldingInputParametersSmallFilteringLevelRadioButtonOnSelection(bool checked);
    void storeArcWeldingInputParametersNoneFilteringLevelRadioButtonOnSelection(bool checked);
    void storeArcWeldingInputParametersResetToNormalBehaviourRadioButtonOnSelection(bool checked);
    void storeArcWeldingInputParametersMaintainOutputBehaviourRadioButtonOnSelection(bool checked);
    void storeArcWeldingOutputAmplitudeCheckBoxOnSelection(bool checked);
    void storeArcWeldingOutputCurrentCheckBoxOnSelection(bool checked);
    void storeArcWeldingOutputFrequencyCheckBoxOnSelection(bool checked);
    void storeArcWeldingOutputTravelSpeedCheckBoxOnSelection(bool checked);
    void storeArcWeldingOutputVoltageCheckBoxOnSelection(bool checked);
    void storeArcWeldingOutputWireFeedCheckBoxOnSelection(bool checked);
    void insertArcWeldingLookUpTableViewOnSelectionChange(const QItemSelection &, const QItemSelection &);
    void saveArcWeldingLookUpTableData();
    void saveReviewBlockDiagramSettingsOnClick();
    void loadReviewBlockDiagramSettingsOnClick();
    void gotoHomeWidgetOnClick();
    void storeArcWeldingOutputWireFeedFilterOnChange(int);
    void storeArcWeldingOutputWireFeedNoMatchOnChange(int);
    void storeArcWeldingOutputVoltageFilterOnChange(int);
    void storeArcWeldingOutputVoltageNoMatchOnChange(int);
    void storeArcWeldingOutputCurrentFilterOnChange(int);
    void storeArcWeldingOutputCurrentNoMatchOnChange(int);
    void storeArcWeldingOutputTravelSpeedFilterOnChange(int);
    void storeArcWeldingOutputTravelSpeedNoMatchOnChange(int);
    void enableBlockDiagramForEditing();
    void saveBlockDiagramAfterEditing();
    void showFinalPageInputWidget();
    void saveFinalPageInputWidgetData();
    void showFinalPageOutput1Widget();
    void showFinalPageOutput2Widget();
    void showFinalPageOutput3Widget();
    void saveFinalPageOutput1Widget();
    void saveFinalPageOutput2Widget();
    void saveFinalPageOutput3Widget();
    void showFinalPagelutWidget();
    void saveFinalPagelutWidgetData();
    void insertFinalPageLookUpTableViewOnSelectionChange(const QItemSelection &, const QItemSelection &);
    void callTinyXMLApiToGenerateAdapXML();

private:
    Ui::AdapHmiMainWindow *ui;
    adaphmiwidgetstate *reviewBlockDiagramWeldingParametersWidgetState;
    adaphmiwidgetstate *arcWeldingParametersWidgetState;
    adaphmiwidgetstate *arcWeldingAdditionalParameterWidgetState;
    adaphmiwidgetstate *reviewBlockDiagramSettingsWidgetState;
    adaphmiwidgetstate *adapParametersSettingsWidgetState;
    adaphmiwidgetstate *arcWeldingInputParametersVisionInputWidgetState;
    adaphmiwidgetstate *arcWeldingInputParametersFilteringLevelWidgetState;
    adaphmiwidgetstate *arcWeldingInputParametersNoMatchBehaviourWidgetState;
    adaphmiwidgetstate *arcWeldingOutputParametersBodyWidgetState;
    adaphmiwidgetstate *arcWeldingOutputAdditionalParameterWidgetState;
    adaphmiwidgetstate *arcWeldingLookUpTableViewState;
    adaphmiwidgetstate *arcWeldingLookUpTableState;
    adaphmiwidgetstate *adapHmiLiveModeState;
    QMap<QString,QString> lutInputOutputMap;
    QMap<adaphmisavelookuptable*,QString> lutMap;
    QStandardItemModel *lutModel;
    adaphmisaveload *saveAdapHmiUserInterfaceState;
    adaphmisaveload *loadAdapHmiUserInterfaceState;
    adaphmiwidgetstate *finalPageInputWidgetState;
    QPushButton *finalPageInputPushButton1;
    QPushButton *finalPageInputPushButton2;
    QPushButton *finalPageInputPushButton3;
    adaphmiwidgetstate *finalPageInputPushButton1State;
    adaphmiwidgetstate *finalPageInputPushButton2State;
    adaphmiwidgetstate *finalPageInputPushButton3State;
    QPushButton *finalPageOutputPushButton1;
    QPushButton *finalPageOutputPushButton2;
    QPushButton *finalPageOutputPushButton3;
    adaphmiwidgetstate *finalPageOutputPushButton1State;
    adaphmiwidgetstate *finalPageOutputPushButton2State;
    adaphmiwidgetstate *finalPageOutputPushButton3State;
    adaphmiwidgetstate *finalPageOutput1WidgetState;
    adaphmiwidgetstate *finalPageOutput2WidgetState;
    adaphmiwidgetstate *finalPageOutput3WidgetState;
    QMap<int,QString> identifyOutputMap;
    adaphmiwidgetstate *finalPageLookUpTableState;
    int outputCount;
};

#endif // ADAPHMIMAINWINDOW_H
