#include "tcviewstockgroup.h"

#include "tcviewentitygroup.h"
#include "tcviewquantitygroup.h"

#define ENTITY_OFFSET 300

tcViewStockGroup::tcViewStockGroup(QObject *pParent, tcStock *pStock, tcViewModel *pViewModel)
	: QObject(pParent)
{
	mStock = pStock;

	int i;
	//create the entity group, and the quantity group
	QList<tcViewEntityGroup*> entitylist;
	QList<tcViewQuantityGroup*> quantitylist;
	qreal minentityval = 0.0, maxentityval = 0.0;
	qreal maxquantityval = 0.0;
	for (i=0; i<pViewModel->GetViewItemCount(); i++) {
		tcViewModelStockData *data = pViewModel->GetViewItemData(i, mStock);
		Q_ASSERT(data);
		tcViewEntityGroup *entitygroup = new tcViewEntityGroup(this, data);
		entitylist.append(entitygroup);
		if (data->MinPrice != 0.0 && (data->MinPrice < minentityval || minentityval == 0.0)) {
			minentityval = data->MinPrice;
		}
		if (data->MaxPrice != 0.0 && data->MaxPrice > maxentityval) {
			maxentityval = data->MaxPrice;
		}
		tcViewQuantityGroup *quantitygroup = new tcViewQuantityGroup(this, data);
		quantitylist.append(quantitygroup);
		if (data->Quantity != 0 && data->Quantity > maxquantityval) {
			maxquantityval = data->Quantity;
		}
	}
	//reposition and insert entity into this
	if (maxentityval != 0.0) {
		i = 0;
		foreach(tcViewEntityGroup* entitygroup, entitylist) {
			qreal scale = (pViewModel->GetViewEntityHeight()) / (maxentityval-minentityval);
			entitygroup->SetPosAndScale(i * pViewModel->GetViewItemWidth(), minentityval * scale - pViewModel->GetViewEntityOffset(), scale);
			this->addToGroup(entitygroup);
			i++;
		}
	}
	//reposition and insert quantity into this
	if (maxquantityval != 0.0) {
		i = 0;
		foreach(tcViewQuantityGroup* quantitygroup, quantitylist) {
			quantitygroup->SetPosAndScale(i * pViewModel->GetViewItemWidth(), (pViewModel->GetViewQuantityHeight()) / maxquantityval);
			this->addToGroup(quantitygroup);
			i++;
		}
	}
}

#ifdef WIN32
	#include "moc_tcviewstockgroup.cpp"
#endif
