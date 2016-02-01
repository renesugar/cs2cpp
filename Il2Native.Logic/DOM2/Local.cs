﻿namespace Il2Native.Logic.DOM2
{
    using System.Runtime.Serialization;

    using Microsoft.CodeAnalysis;
    using Microsoft.CodeAnalysis.CSharp;
    using Microsoft.CodeAnalysis.CSharp.Symbols;

    public class Local : Expression
    {
        private static ObjectIDGenerator objectIDGenerator = new ObjectIDGenerator();

        private string customName;

        public ILocalSymbol LocalSymbol { get; set; }

        internal static void WriteLocal(ILocalSymbol local, CCodeWriterBase c)
        {
            c.WriteName(local);
        }

        internal void Parse(BoundLocal boundLocal)
        {
            base.Parse(boundLocal);
            Parse(boundLocal.LocalSymbol);
        }

        internal void Parse(LocalSymbol localSymbol)
        {
            Type = localSymbol.Type;
            IsReference = this.Type.IsReferenceType;

            ParseName(localSymbol);
            this.LocalSymbol = localSymbol;
        }

        internal override void WriteTo(CCodeWriterBase c)
        {
            if (this.customName != null)
            {
                c.TextSpan(this.customName);
            }
            else
            {
                WriteLocal(this.LocalSymbol, c);
            }
        }

        private void ParseName(LocalSymbol local)
        {
            if (local.SynthesizedLocalKind != SynthesizedLocalKind.None)
            {
                var lbl = string.Empty;
                if (local.SynthesizedLocalKind > SynthesizedLocalKind.ForEachArrayIndex0 &&
                    local.SynthesizedLocalKind < SynthesizedLocalKind.ForEachArrayLimit0)
                {
                    lbl = string.Format("ForEachArrayIndex{0}", local.SynthesizedLocalKind - SynthesizedLocalKind.ForEachArrayIndex0);
                }
                else if (local.SynthesizedLocalKind > SynthesizedLocalKind.ForEachArrayLimit0 &&
                    local.SynthesizedLocalKind < SynthesizedLocalKind.FixedString)
                {
                    lbl = string.Format("ForEachArrayLimit{0}", local.SynthesizedLocalKind - SynthesizedLocalKind.ForEachArrayLimit0);
                }
                else
                {
                    lbl = local.SynthesizedLocalKind.ToString();
                }

                if (local.SynthesizedLocalKind == SynthesizedLocalKind.LoweringTemp)
                {
                    var firstTime = false;
                    lbl += string.Format("_{0}", objectIDGenerator.GetId(local, out firstTime).ToString());
                }

                this.customName = lbl;
            }
        }
    }
}
