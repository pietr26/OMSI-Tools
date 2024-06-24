#ifndef OCTICKETPACK_H
#define OCTICKETPACK_H

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
            bool isTicket2;
            QString nameDeu;
            QString nameEng;
            int maxStops;
            int minimumAge;
            int maximumAge;
            float price;
            QString displayText;

            // if(isTicket2) {
            bool isDayTicket;
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
