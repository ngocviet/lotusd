// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2017-2019 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/test/uritests.h>

#include <chainparams.h>
#include <config.h>
#include <qt/guiutil.h>
#include <qt/walletmodel.h>

#include <QUrl>

void URITests::uriTestsLotus() {
    const auto params = CreateChainParams(CBaseChainParams::MAIN);

    SendCoinsRecipient rv;
    QString scheme = QString("payto");
    QUrl uri;
    uri.setUrl(QString(
        "lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ?req-dontexist="));
    QVERIFY(!GUIUtil::parseBitcoinURI(scheme, uri, &rv));

    uri.setUrl(QString(
        "payto:lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ?dontexist="));
    QVERIFY(GUIUtil::parseBitcoinURI(scheme, uri, &rv));
    QVERIFY(rv.address ==
            QString("lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == Amount::zero());

    uri.setUrl(
        QString("payto:lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ?label="
                "Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(scheme, uri, &rv));
    QVERIFY(rv.address ==
            QString("lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ"));
    QVERIFY(rv.label == QString("Wikipedia Example Address"));
    QVERIFY(rv.amount == Amount::zero());

    uri.setUrl(QString(
        "payto:lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ?amount=100000"));
    QVERIFY(GUIUtil::parseBitcoinURI(scheme, uri, &rv));
    QVERIFY(rv.address ==
            QString("lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100000 * SATOSHI);

    uri.setUrl(QString("payto:lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ?"
                       "amount=1001000"));
    QVERIFY(GUIUtil::parseBitcoinURI(scheme, uri, &rv));
    QVERIFY(rv.address ==
            QString("lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 1001000 * SATOSHI);

    uri.setUrl(QString("payto:lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ?"
                       "amount=100000000&"
                       "label=Wikipedia Example"));
    QVERIFY(GUIUtil::parseBitcoinURI(scheme, uri, &rv));
    QVERIFY(rv.address ==
            QString("lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ"));
    QVERIFY(rv.amount == int64_t(100000000) * SATOSHI);
    QVERIFY(rv.label == QString("Wikipedia Example"));

    uri.setUrl(
        QString("payto:lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ?message="
                "Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(scheme, uri, &rv));
    QVERIFY(rv.address ==
            QString("lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ"));
    QVERIFY(rv.label == QString());

    QVERIFY(GUIUtil::parseBitcoinURI(
        scheme,
        "payto://"
        "lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ?"
        "message=Wikipedia Example Address",
        &rv));
    QVERIFY(rv.address ==
            QString("lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ"));
    QVERIFY(rv.label == QString());

    uri.setUrl(QString(
        "payto:lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ?req-message="
        "Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(scheme, uri, &rv));

    uri.setUrl(QString(
        "payto:lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ?amount=1000000,"
        "000&label=Wikipedia Example"));
    QVERIFY(!GUIUtil::parseBitcoinURI(scheme, uri, &rv));

    uri.setUrl(QString(
        "payto:lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ?amount=1000000,"
        "000.0&label=Wikipedia Example"));
    QVERIFY(!GUIUtil::parseBitcoinURI(scheme, uri, &rv));
}

void URITests::uriTestsCashAddr() {
    const auto params = CreateChainParams(CBaseChainParams::MAIN);

    SendCoinsRecipient rv;
    QUrl uri;
    QString scheme = QString::fromStdString(params->CashAddrPrefix());
    uri.setUrl(QString("bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?"
                       "req-dontexist="));
    QVERIFY(!GUIUtil::parseBitcoinURI(scheme, uri, &rv));

    uri.setUrl(QString(
        "bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?dontexist="));
    QVERIFY(GUIUtil::parseBitcoinURI(scheme, uri, &rv));
    QVERIFY(rv.address ==
            QString("bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == Amount::zero());

    uri.setUrl(
        QString("bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?label="
                "Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(scheme, uri, &rv));
    QVERIFY(rv.address ==
            QString("bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a"));
    QVERIFY(rv.label == QString("Wikipedia Example Address"));
    QVERIFY(rv.amount == Amount::zero());

    uri.setUrl(QString("bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?"
                       "amount=100000"));
    QVERIFY(GUIUtil::parseBitcoinURI(scheme, uri, &rv));
    QVERIFY(rv.address ==
            QString("bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100000 * SATOSHI);

    uri.setUrl(QString("bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?"
                       "amount=1001000"));
    QVERIFY(GUIUtil::parseBitcoinURI(scheme, uri, &rv));
    QVERIFY(rv.address ==
            QString("bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 1001000 * SATOSHI);

    uri.setUrl(QString("bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?"
                       "amount=10000000000&"
                       "label=Wikipedia Example"));
    QVERIFY(GUIUtil::parseBitcoinURI(scheme, uri, &rv));
    QVERIFY(rv.address ==
            QString("bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a"));
    QVERIFY(rv.amount == int64_t(10000000000) * SATOSHI);
    QVERIFY(rv.label == QString("Wikipedia Example"));

    uri.setUrl(QString(
        "bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?message="
        "Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(scheme, uri, &rv));
    QVERIFY(rv.address ==
            QString("bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a"));
    QVERIFY(rv.label == QString());

    QVERIFY(
        GUIUtil::parseBitcoinURI(scheme,
                                 "bitcoincash://"
                                 "qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?"
                                 "message=Wikipedia Example Address",
                                 &rv));
    QVERIFY(rv.address ==
            QString("bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a"));
    QVERIFY(rv.label == QString());

    uri.setUrl(QString(
        "bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?req-message="
        "Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(scheme, uri, &rv));

    uri.setUrl(QString(
        "bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?amount=1,"
        "000&label=Wikipedia Example"));
    QVERIFY(!GUIUtil::parseBitcoinURI(scheme, uri, &rv));

    uri.setUrl(QString(
        "bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a?amount=1,"
        "000.0&label=Wikipedia Example"));
    QVERIFY(!GUIUtil::parseBitcoinURI(scheme, uri, &rv));
}

void URITests::uriTestFormatURI() {
    const auto params = CreateChainParams(CBaseChainParams::MAIN);

    {
        SendCoinsRecipient r;
        r.address = "bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a";
        r.message = "test";
        QString uri = GUIUtil::formatBitcoinURI(*params, r);

        QVERIFY(uri == "lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ?"
                       "message=test");
    }

    {
        SendCoinsRecipient r;
        r.address = "lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ";
        r.message = "test";
        QString uri = GUIUtil::formatBitcoinURI(*params, r);

        QVERIFY(uri == "lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ?"
                       "message=test");
    }

    {
        // Garbage goes through (address checksum is invalid)
        SendCoinsRecipient r;
        r.address = "175tWpb8K1S7NmH4Zx6rewF9WQrcZv245W";
        r.message = "test";
        QString uri = GUIUtil::formatBitcoinURI(*params, r);
        QVERIFY(uri == "175tWpb8K1S7NmH4Zx6rewF9WQrcZv245W?message=test");
    }

    {
        // Legacy addresses are converted.
        SendCoinsRecipient r;
        r.address = "12c6DSiU4Rq3P4ZxziKxzrL5LmMBrzjrJX";
        r.message = "test";
        QString uri = GUIUtil::formatBitcoinURI(*params, r);
        QVERIFY(uri == "lotus_16PSJHejVUZd1LPr51EAB4zK1Xi3D5qjQNtNHkof5?"
                       "message=test");
    }
}
