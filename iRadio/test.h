/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: xxx
 * Author: zhupingfang
 * Create: 2020-10-8
 * Note  : add ShowNetsInfo() for ConstraintEdit by liminglei 2021-7-19
 */
#ifndef ELEMENTS_REPORT_DIALOG_VIEW_CTRL_H
#define ELEMENTS_REPORT_DIALOG_VIEW_CTRL_H

#include "info_api.h"
#include "view_ctrl.h"
#include "view_ctrl_common.h"
#include "elements_report_dialog.h"
#include "cmd_common.h"

class ElementsReportDialogViewCtrl : public ViewCtrl {
    Q_OBJECT
public:
    static ElementsReportDialogViewCtrl& GetInst();

    ~ElementsReportDialogViewCtrl() override;
    void MakeTopWidget() override;

    void Initial(
        QWidget& parent,
        const std::vector< std::map< LayoutEng::FindType,
                                     std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
            values);
    void ShowInfoDialog(
        const std::vector< std::map< LayoutEng::FindType,
                                     std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
            infoValues);
    void CloseDialog();
    // show the infomation of net, add by liminglei 2021-7-17
    void ShowNetsInfo(const std::vector< std::string >& netNames,
                      bool status                                            = false,
                      bool isDoubleClickPPR                                  = false,
                      std::vector< DesignDatabase::DataObjectId > pinpairIds = {0});
    void SelectNets(const std::vector< std::string >& netNames,
                    bool status                                            = false,
                    bool isDoubleClickPPR                                  = false,
                    std::vector< DesignDatabase::DataObjectId > pinpairIds = {0});
    void SelecrtDrcVio(DesignDatabase::DataObjectId drcId);  // add by remwei 2021-7-26
    void DrcCloseAndOpenLayers(DesignDatabase::DataObjectId drcId);
    void GetElementLayers(DesignDatabase::DataObjectId drcId,
                          const std::vector< DesignDatabase::DataObjectId >& allVisibleLayers,
                          std::vector< uint64_t >& layersVec);
    void GetLayerIdByNetId(DesignDatabase::DataObjectId netId,
                           std::set< DesignDatabase::DataObjectId >& elementLayerIds);
    void GetLayerIdByElementId(const std::vector< DesignDatabase::DataObjectId >& elementId,
                               std::set< DesignDatabase::DataObjectId >& elementLayerIds);
    void GetAllDrcPinViaLayerId(const std::vector< DesignDatabase::DataObjectId >& violationIds,
                                const std::vector< DesignDatabase::DataObjectId >& allVisibleLayers,
                                std::set< DesignDatabase::DataObjectId >& openLayerIds);
    void GetPinViaConnectRelationLayer(DesignDatabase::DataObjectId eleId,
                                       const std::vector< DesignDatabase::DataObjectId >& allVisibleLayers,
                                       std::set< DesignDatabase::DataObjectId >& elementLayerIds);
    void GetOtherElementLayerId(DesignDatabase::DataObjectId eleId,
                                std::set< DesignDatabase::DataObjectId >& elementLayerIds);
    void GetNotAllDrcElementLayer(const std::string& drcLayerName,
                                  const std::vector< DesignDatabase::DataObjectId >& violationIds,
                                  std::set< DesignDatabase::DataObjectId >& elementLayerIds);
    void HighLightConsNetName(DesignDatabase::DataObjectId drcId);  // add by anling 2023/01/10   use to set constraint
                                                                    // net page netName back color.
    void GetNetIdByObjId(uint64_t objId, uint64_t& netId);          // add by anling 2023/01/10
    bool GetElementDigStatus();
    void GetAndShowNetsInfo(const std::vector< std::string >& netNames);
    void HighlightDrcVio(DesignDatabase::DataObjectId DrcId);  // add by remwei 2021-7-26

private:
    void InitialConnect(ElementsReportDialog* dialog);
    ElementsReportDialogViewCtrl();
    bool ItemRelatedNet(const LayoutEng::FindType& type);
    bool ElementsValueChecked(
        std::vector< std::map< LayoutEng::FindType, std::map< std::string, std::string > > >& infoValues);
    void GetDefaultFilePath(QString& filePath);
    // show the infomation of net, add by liminglei 2021-7-17
    void HighlightNet(const std::vector< std::string >& netNames, bool status = false);
    void HighlightPinpair(DesignDatabase::DataObjectId pinpairId);
    QRectF ShowRectInCentre(const Rect& showRect);
    void ExpandNetRectByRect(const std::string& netName, Rect& showRect);
    std::vector< DesignDatabase::DataObjectId > GetNetIds(const std::vector< std::string >& netNames);
    uint32_t GetViewMoveLocation(DesignDatabase::DataObjectId DrcId, QPointF& point);  // add by fengjiabao 2024-06-13
    void LockElement(const std::string& netName);                                      // add by remwei 2021-8-2
    void UnlockElement(const std::string& netName);                                    // add by remwei 2021-8-2
    uint32_t GetLockInfo(const std::string& netName,
                         std::vector< std::vector< DesignDatabase::DataObjectId > >& vecList);
    void RecordCloseElementsReportDialog();
    void InitialCliFnCf();
    void CfgCloseCmd(const std::string& value);
    void ProcessCliCmd(const std::string& optName, const std::string& value);
    void ProcessHilightCmd(const std::vector< std::string >& netNames);
    // add by anling
    void InitInfoKeyWordsOne();
    void InitInfoKeyWordsTwo();
    void InitInfoKeyWordsThr();
    void InitTextValue(uint32_t index, std::map< LayoutEng::FindType, QString >& textValue);
    void InitListing(uint32_t index,
                     const LayoutEng::FindType&,
                     const QString&,
                     std::map< LayoutEng::FindType, QString >& textValue);
    QString TransTextAndEnd(QString const& data);
    QString TransNextLineAndEnd(QString const& data);
    QString TransStartText(QString const& data);
    QString TransNoKeyStartText(QString const& data);
    void AddRowData(QString& text, const QString& firstPar, const QString& secondPar);
    void AddSpaceRowData(QString& text, const QString& firstPar, const QString& secondPar);
    QString TransHyperLink(QString const& head, QString const& link, QString const& doId);
    QString TransLine();
    QString TransPinListStart(QString const& data);
    QString TransPinListEnd(QString const& data);
    void HighLight(const QUrl& link);
    void GetTypeListCount(
        uint32_t index,
        const std::vector< std::map< LayoutEng::FindType,
                                     std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
            values);

    void InitTypeCountFuncMap();
    void GetTypeCount(const LayoutEng::FindType& type,
                      const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData);
    void GroupsTypeCount(
        const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData);
    void ComponentsTypeCount(
        const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData);
    void CellsTypeCount(const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData);
    void ENetsTypeCount(const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData);
    void NetsTypeCount(const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData);
    void PinsTypeCount(const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData);
    void ViasTypeCount(const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData);
    void GuidelinesTypeCount(
        const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData);
    void TracesTypeCount(
        const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData);
    void LinesTypeCount(const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData);
    void TraceSegsTypeCount(
        const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData);
    void OtherSegsTypeCount(
        const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData);
    void AreasTypeCount(const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData);
    void CutoutsTypeCount(
        const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData);
    void TextsTypeCount(const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData);
    void DRCMarksTypeCount(
        const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData);
    void DimensionTypeCount(
        const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData);

    void DoWithNormalsInfo(const LayoutEng::ShowInfos::InfoKeyWords& keyWords, const QString& data, QString& text);
    void DoWithIncludePointInfo(const LayoutEng::ShowInfos::InfoKeyWords& keyWords,
                                const std::vector< LayoutEng::ShowInfos::InfoChildData >& info,
                                QString& text);
    void NoPointShardsInfo(const LayoutEng::ShowInfos::InfoChildData& childData,
                           const int& infoSize,
                           const int& count,
                           const bool& isHaveKeyWords,
                           QString& text);
    void PointShardsInfo(const LayoutEng::ShowInfos::InfoChildData& childData,
                         const int& infoSize,
                         const int& count,
                         const bool& isHaveKeyWords,
                         QString& text);
    void DoWithNormalPointInfo(const LayoutEng::ShowInfos::InfoKeyWords& keyWords,
                               const QString& data,
                               const DesignDatabase::DataObjectId& id,
                               QString& text);
    void DoWithElectricalInfo(const QString& data, QString& text);
    void DoWithPinListValue(const LayoutEng::ShowInfos::InfoKeyWords& keyWords,
                            const std::vector< LayoutEng::ShowInfos::InfoChildData >& info,
                            QString& text);
    void InitValueFuncMap();
    void GetTypeValue(const LayoutEng::FindType& type,
                      const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& values,
                      QString& text);
    void NetsTypeValue(const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& values,
                       QString& text);
    void GuidelinesTypeValue(
        const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& values, QString& text);
    void TracesOrLinesTypeValue(
        const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& values, QString& text);
    void AreasOrCutoutTypeValue(
        const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& values, QString& text);
    void OtherOrTraceSegTypeValue(
        const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& values, QString& text);
    void NormalPointTypeValue(
        const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& values, QString& text);
    void NormalTypeValue(const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& values,
                         QString& text);
    void MakeHtmlText(QString& htmlText, const QString& addText);

    bool AreaIsBoundaryLayer(
        const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& values);
    void ClassifiedDataByType(
        const std::vector< std::map< LayoutEng::FindType,
                                     std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
            allInfo,
        std::map< LayoutEng::FindType,
                  std::vector< std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
            infoMapByType);
    void ElementTypeChange(const LayoutEng::FindType& currentType, uint32_t index);
    QString GetNextDataForShowOnce(
        uint32_t index,
        const LayoutEng::FindType& currentType,
        const std::map< LayoutEng::FindType,
                        std::vector< std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
            infoMapByType,
        bool isNewOnceDisplay = false);
    QString GetCountHtmlForShowFirst(uint32_t index, const LayoutEng::FindType& currentType);
    void UpdateNextPageData(LayoutEng::FindType currentType, uint32_t index);
    void UpdatePreviousPageData(LayoutEng::FindType currentType, uint32_t index);
    QString GetPreviousDataForShowOnce(
        const LayoutEng::FindType& currentType,
        const std::map< LayoutEng::FindType,
                        std::vector< std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
            infoMapByType,
        uint32_t index);
    void SetPageBtnEnableAfterTurnLeft(uint32_t index);
    void SetPageBtnEnableAfterTurnRight(uint32_t index,
                                        const uint32_t lastDisplayCount,
                                        const uint32_t allElementCount,
                                        bool isNewOnceDisplay = false);
    void SetAllTypeValuesForSave(
        const std::vector< std::map< LayoutEng::FindType,
                                     std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
            values,
        uint32_t index);
    void SaveInfoFile(uint32_t index);
    void SliceData(uint32_t index, const QString& targetHtml, std::vector< QString >& sliceDatas);
    void IntialSliceData(uint32_t index);
    QString GetNextSliceData(const uint32_t& allElementCount, uint32_t index, QString& onceHtml);
    QString GetPreviousSliceData(const LayoutEng::FindType& currentType, uint32_t index, QString& onceHtml);
    bool IsDisplayCountDataForRight(uint32_t index);
    bool IsDisplayCountDataForLeft(uint32_t index);
    void SortOutBoundaryInfo(
        const LayoutEng::FindType& currentType,
        std::map< LayoutEng::FindType,
                  std::vector< std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
            infoMapByType);
    void CloseElementsReportDialog(uint32_t index);
    ElementsReportDialog* GetDialogByIndex(uint32_t index);
    uint32_t GetLastDialogIndex(const std::list< ElementsReportDialog* >& dialogList);
    ElementsReportDialog* GetLastDialog(const std::list< ElementsReportDialog* >& dialogList);
    void RecordingCloseDialog();
    void NewElementsReportDialog(ElementsReportDialog* dialog);
    void InitToSetData(ElementsReportDialog* dialog);
    void ClickFnCf();
    void RecordDlgFixedStatus(bool isDialogFixed, uint32_t index);
    void CmdChangeDlgFixedStatus(const std::string& value);
    void UpdateDialogAllInfo();
    void DeleteDialogAllInfo(uint32_t index);
    void MoveLastDialogPos();
    void MoveNewDialogPos(ElementsReportDialog* dialog);
    void SetInfoMapByType(
        uint32_t index,
        std::map< LayoutEng::FindType,
                  std::vector< std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
            infoMapByType);
    void GetInfoMapByType(
        uint32_t index,
        std::map< LayoutEng::FindType,
                  std::vector< std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
            infoMapByType);
    void CloseAllDialog();
    void SetAllSliceDatas(uint32_t index, std::vector< QString >& sliceDatas);
    std::vector< QString > GetSliceData(uint32_t index);
    void SetCurrentSliceNumber(uint32_t index, uint32_t currentSliceNumber);
    uint32_t GetCurrentSliceNumber(uint32_t index);
    void ClearDisplayCount(uint32_t index);
    uint32_t GetFirstDisplayCount(uint32_t index);
    uint32_t GetLastDisplayCount(uint32_t index);
    void SetFirstDisplayCount(uint32_t index, uint32_t displayCount);
    void SetLastDisplayCount(uint32_t index, uint32_t displayCount);
    void InitalSliceData(uint32_t index);
    uint32_t SplittingLink(const QUrl& link, DesignDatabase::DataObjectId& id, QStringList& point);
    QString GetPreviousOnceHtml(uint32_t index,
                                const LayoutEng::FindType& currentType,
                                QString& tempHtml,
                                QString& onceHtml);
    uint32_t UpdateAllInfoMapByType(
        uint32_t index,
        std::map< LayoutEng::FindType,
                  std::vector< std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
            infoMapByType,
        std::vector< std::map< LayoutEng::FindType,
                               std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
            allInfo);
    void SetElectricalHtmlText(int i, QStringList& propertyInfoList, QString& text);
    void SetNormalsHtmlText(bool isHaveKeyWords, const QString& data, QString& text);
    QString GetNextOnceHtml(
        uint32_t index, uint32_t size, bool isNewOnceDisplay, QString& tempHtml, QString& countHtml, QString& onceHtml);
    void GetInfoByViolationIds(std::vector< DesignDatabase::DataObjectId >& violationIds,
                               DesignDatabase::DataObjectId& netId);

private:
    using FuncDef = void (ElementsReportDialogViewCtrl::*)(const std::string& value);
    std::map< std::string, FuncDef > funcMap_;
    // add by anling
    std::vector< std::map< LayoutEng::FindType,
                           std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >
        allInfo_ {};
    std::map< LayoutEng::ShowInfos::InfoKeyWords, std::string > infoKeyWords_;
    std::map< LayoutEng::FindType, int > infoListNumber_;
    using typeFuncDef = void (ElementsReportDialogViewCtrl::*)(
        const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >&);
    std::map< LayoutEng::FindType, typeFuncDef > switchCountFuncMap_;
    using valueFuncDef = void (ElementsReportDialogViewCtrl::*)(
        const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >&, QString&);
    std::map< LayoutEng::FindType, valueFuncDef > switchValueFuncMap_;
    std::list< ElementsReportDialog* > dialogList_ {};
    QPoint popUpDlgPos_ {0, 0};  // 记录最后一个关闭info窗口的位置
    std::list< std::pair<
        uint32_t,
        std::vector< std::map< LayoutEng::FindType,
                               std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > > > >
        dialogAllInfo_ {};  // 不同info窗口导出文件内容不同
    std::list< std::pair< uint32_t, std::map< LayoutEng::FindType, int > > > dialogInfoListNumber_ {};
    bool lastDialogIsClose_ = false;  // 记录最后一个info窗口是否关闭
    std::list< std::pair<
        uint32_t,
        std::map< LayoutEng::FindType,
                  std::vector< std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > > > >
        allInfoMapByType_ {};
    std::list< std::pair< uint32_t, std::vector< QString > > > allSliceDatas_ {};
    std::list< std::pair< uint32_t, uint32_t > > allCurrentSliceNumber_ {};  // 当前显示到切片数据的第几片数
    std::list< std::pair< uint32_t, uint32_t > > allLastDisplayCount_ {};  //当前页显示的最后一个对象的下一个对象数
    std::list< std::pair< uint32_t, uint32_t > > allFirstDisplayCount_ {};  // 当前页显示的第一个对象数
};
#endif  // ELEMENTS_REPORT_DIALOG_VIEW_CTRL_H
