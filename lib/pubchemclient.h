#ifndef PUBCHEMCLIENT_H
#define PUBCHEMCLIENT_H

#include <QtNetwork>
#include <QtGui>

class CompoundRecord 
{
	public:
		QString molecularFormula;
		QString molecularWeight;
		QString iupacName;
		QPixmap image;

};

class PubChemClient 
{
	public:
		PubChemClient(QObject *parent);

		CompoundRecord* getRecordByName(QString name);

	private:
		QNetworkReply* sendRequest(QString url);
		QString formatMolecularFormula(QString input);

		QObject *parent;
		QNetworkAccessManager manager;
		QEventLoop loop;
};

#endif