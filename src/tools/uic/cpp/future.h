#ifndef __CPP_FUTURE_H__
#define __CPP_FUTURE_H__

QT_BEGIN_NAMESPACE

namespace {
    static bool hasFutureProperty(const CPP::WriteInitialization::DomPropertyList &lst) {
        for (int i=0; i<lst.size(); ++i) {
            const DomProperty *p = lst.at(i);
            QString propertyName = p->attributeName();
            if (propertyName != QLatin1String("future")) {
                continue;
            }
            if (p->kind() != DomProperty::Bool) {
                qWarning("### Ignoring non-bool future");
                return false;
            }
            QString propertyValue = p->elementBool();
            return propertyValue == QLatin1String("true");
        }
        return false;
    }
}

QT_END_NAMESPACE

#endif
