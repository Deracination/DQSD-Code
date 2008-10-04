#pragma once

class HtmlElement
{
public:
    HtmlElement(IHTMLElement* pDOMElement)
        : m_spDOMElement(pDOMElement)
    {
    }

    HRESULT GetAttribute(const wchar_t* name, std::string& value)
    {
        CComVariant var;
        HRESULT hr = GetAttribute(name, VT_BSTR, &var);
        if (FAILED(hr)) return hr;

        if (var.bstrVal == NULL)
            value = std::string();
        else
            value = CW2A(var.bstrVal);

        return S_OK;
    }

    HRESULT GetAttribute(const wchar_t* name, bool& value)
    {
        CComVariant var;
        HRESULT hr = GetAttribute(name, VT_BOOL, &var);
        if (FAILED(hr)) return hr;

        value = (var.boolVal == VARIANT_TRUE);
        return S_OK;
    }

    HRESULT GetTagName(std::string& value)
    {
        CComBSTR tagName;
        HRESULT hr = m_spDOMElement->get_tagName(&tagName);
        if (FAILED(hr)) return hr;

        value = CW2A(tagName);
        return S_OK;
    }

private:
    HRESULT GetAttribute(const wchar_t* name, VARTYPE expectedType, VARIANT* pvalue)
    {
        CComVariant value;
        HRESULT hr = m_spDOMElement->getAttribute(CComBSTR(name), 0, &value);
        if (FAILED(hr)) return hr;

        if (value.vt != expectedType)
            return DISP_E_TYPEMISMATCH;

        return value.Detach(pvalue);
    }

    CComPtr<IHTMLElement> m_spDOMElement;
};