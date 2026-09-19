#ifndef OCTICKETPACK_H
#define OCTICKETPACK_H


#include <QList>
#include <QString>
#include "OCBase.h"

class OCTicketPack
{
public:
    class SingleTicketPack
    {
    public:
        class Ticket
        {
        public:
            bool isTicket2 = false;
            QString nameDeu;
            QString nameEng;
            int maxStops = 0;
            int minimumAge = 0;
            int maximumAge = 0;
            float price = 0.0f;
            QString displayText;

            // if(isTicket2) {
            bool isDayTicket = false;
            float maxTicketPurchaseFactor = -1;
            // }
        };

        float devaluationFactor = -1;
        float PurchaseFactor = -1;
        float talkFactor = -1;
        float complainFactor = 1;

        QList<Ticket> tickets;
    };

    QString voicePath;
    QList<SingleTicketPack> ticketPacks;
};

#endif // OCTICKETPACK_H
